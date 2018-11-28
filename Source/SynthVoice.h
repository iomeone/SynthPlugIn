/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Oct 2017 10:18:29am
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "MaxiOsc.h"
#include "EnvelopeGenerator.h"
#include "MaxiDelay.h"
#include "SVFDspUtilities.h"
#include "SVFStateVariableFilter.h"
#include "ModalUnit.h"

#include "UsedParameters.h"

class SynthVoice : public SynthesiserVoice
{
public:
	SynthVoice(AudioProcessorValueTreeState* parameters)
		: parametersPointer(parameters)
	{
        updateAllParameters();
        constructModalUnits();
	}

    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }
    
	//=======================================================

	void setCurrentPlaybackSampleRate(double newRate) override
	{
        mSampleRate = newRate;
        
		SynthesiserVoice::setCurrentPlaybackSampleRate(mSampleRate);

		aDSR.setSampleRate(mSampleRate);
		envImpulse.setSampleRate(mSampleRate);
        delayLine.setSampleRate(mSampleRate, 2.0);
		testOsc.setSampleRate(mSampleRate);
		noiseOsc.setSampleRate(mSampleRate);
        simpleFilter.setSampleRate(mSampleRate);
        
        constructModalUnits(); //automatically sets sample rate as part of reconstruction.
        
	}
    //=======================================================
    
    double modalOutput()
    {
        for(int i = 0; i < mModalUnits.size(); i ++){
            
            ModalUnit* unit = mModalUnits.getUnchecked(i);
            output = output + unit->getOutput();
        }
           
        output = output * outputScalar;   	//ie. output = output / mOscillators.size();
        return output;
    }

	//=======================================================

	double aDSROutput()
	{
        //return aDSR.nextSample();
        
		//TECHNIQUE FOR REDUCING THE OUTPUT OF THE ADSR TO ONCE EVERY 10 SAMPLES (but need to change envelope internally to add 10 per sample to compensate)
		aDSR.setSampleRate(mSampleRate/samplesPerIncrementVar); //eventSampleRAte

		//DBG("eventSampleRate = " << eventSampleRate << " sampleRate = " << mSampleRate << " samplesPerIncrement = " << samplesPerIncrementVar );

		//if(adsrCounter == 0){
  //          adsrValue = aDSR.nextSample();
		//	adsrCounter = adsrCounter + 1; //ERROR IS HERE - NEXT RUN WILL BE EQUAL TO 2 NOT 1
  //      }
  //      else if(adsrCounter >= samplesPerIncrementVar){
  //          adsrCounter = 0;
  //      }
  //      else {
  //          adsrCounter = adsrCounter + 1;
  //      }
  //      return adsrValue;
		if (adsrCounter >= samplesPerIncrementVar) {
			adsrValue = aDSR.nextSample();
			adsrCounter = 1;
		}
		else {
			adsrCounter++;
		}
		return adsrValue;
	}

	double delayOutput()
	{
        return delayLine.delay(modalOutput()*aDSROutput(), delayTimeVar, delayFeedbackVar, delayPrePostMixVar, delayDWMixVar);
	}


	double filterOutput() //resonant bandpass test
	{
		return simpleFilter.processAudioSample(noiseImpulse(), 1);
	}
	//=======================================================

	double noiseImpulse() //exciter test
	{
		return (envImpulse.nextSample() * noiseOsc.noise()) * 0.3;
	}
	
    //=======================================================
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
		updateParametersOnStartNote();

		//UPDATE MODALUNIT ON START NOTE
		for (int i = 0; i < mModalUnits.size(); i++) {
			ModalUnit* unit = mModalUnits.getUnchecked(i);
			unit->setVelocity(velocity);
			unit->setAttackSeconds(envAttackTime);
			unit->setDecaySeconds(envDecayTime);
			unit->setSustainPercent(envSustainLevel);
			unit->setReleaseSeconds(envReleaseTime);
			unit->setEnterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
			//        unit->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_PREATTACK);
		}

		velocityScaled = velocity;
        newFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        
		simpleFilter.setFilterType(2);
		simpleFilter.setCutoffFreq(300);
		simpleFilter.setQ(1256); // add function setModalQ needs to take decay time in seconds, and note pitch as inputs and multiple by 2 pi

		aDSR.setVelocityValue(velocity);
		aDSR.setAttackSeconds(envAttackTime);
		aDSR.setDecaySeconds(envDecayTime);
		aDSR.setSustainPercent(envSustainLevel);
		aDSR.setReleaseSeconds(envReleaseTime);
        aDSR.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
//        aDSR.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_PREATTACK);
				
		envImpulse.setVelocityValue(velocity);
		envImpulse.setAttackSeconds(0.01);
		envImpulse.setDecaySeconds(0.05);
		envImpulse.setSustainPercent(0.0);
		envImpulse.setReleaseSeconds(0.01);
        envImpulse.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
//        envImpulse.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_PREATTACK);
        
        
	}
    
    //=======================================================
    
    void stopNote (float velocity, bool allowTailOff) override
    {
		aDSR.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		
		//UPDATE MODALUNIT ON STOP NOTE
		for (int i = 0; i < mModalUnits.size(); i++) {
			ModalUnit* unit = mModalUnits.getUnchecked(i);
			unit->setEnterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}

        allowTailOff = false;
        
        if (velocity == 0)
            clearCurrentNote();
    }
    
    //=======================================================
    
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        
    }
    
    //=======================================================
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        
    }
    
    //=======================================================
    
    void renderNextBlock (AudioBuffer <float> &outputBuffer, int startSample, int numSamples) override
    {
		/** update our internal parameters */
		updateParametersEachBlock();

		/** block smoothing */
		// oscillatorGainSmoothed = oscillatorGainSmoothed - 0.01*(oscillatorGainSmoothed - oscillatorGain);
		
		//UPDATE MODALUNIT PER BLOCK
		for (int i = 0; i < mModalUnits.size(); i++) {
			ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setSampleRate(mSampleRate);
				//unit->setEventSampleRate(eventSampleRate);
				unit->setSamplesPerIncrement(samplesPerIncrementVar);
		}

		/** render our audio */
		for (int sample = 0; sample < numSamples; ++sample)
		{
			updateParametersPerSample();

			//SAMPLE SMOOTHING
			oscillatorGainSmoothed = oscillatorGainSmoothed - 0.002*(oscillatorGainSmoothed - synthGain);
            frequency = newFrequency;

			//UPDATE MODALUNIT PER SAMPLE
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
					unit->setFrequency(frequency);
			}
			
			//OUTPUT HERE
			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
                outputBuffer.addSample(channel, startSample, delayOutput()*oscillatorGainSmoothed); ///////MAIN OUTPUT HERE
//                outputBuffer.addSample(channel, startSample, modalUnitOutput()*oscillatorGainSmoothed);
			}
			++startSample;
		}
	
    }
	
	void updateParametersPerSample()
	{
	}

	void updateParametersEachBlock()
	{
		float* inGain = parametersPointer->getRawParameterValue(id_SynthGain);
			if (synthGain != *inGain) {
			synthGain = *inGain;
			}

		float* delayTimePtr = parametersPointer->getRawParameterValue(id_DelTime); 
		delayTimeVar = *delayTimePtr; 

		float* delayFeedbackPtr = parametersPointer->getRawParameterValue(id_DelFeedback);
		delayFeedbackVar = *delayFeedbackPtr;  

		float* delayDWMixPtr = parametersPointer->getRawParameterValue(id_DelDWMix);
		delayDWMixVar = *delayDWMixPtr; 
		
		float* delayPrePostMixPtr = parametersPointer->getRawParameterValue(id_DelPrePostMix);
		delayPrePostMixVar = *delayPrePostMixPtr;
        
        float* numPartialsPtr = parametersPointer->getRawParameterValue(id_NumPartials);
			if(numPartialsVar != *numPartialsPtr){
			numPartialsVar = *numPartialsPtr;
			constructModalUnits();
			}
	
		float* numVoicesPtr = parametersPointer->getRawParameterValue(id_NumVoices); //WHAT TO DO?

		float* samplesPerIncrementPtr = parametersPointer->getRawParameterValue(id_SamplesPerIncrement); 
			if (samplesPerIncrementVar != *samplesPerIncrementPtr) {
				samplesPerIncrementVar = *samplesPerIncrementPtr;
				eventSampleRate = mSampleRate / samplesPerIncrementVar;
			}


	}

	void updateParametersOnStartNote()
	{
		float* envAttackTimePtr = parametersPointer->getRawParameterValue(id_EnvAttack); //IS THIS NECESSARY/CORRECT?
		envAttackTime = *envAttackTimePtr;  //IS THIS NECESSARY/CORRECT?

		float* envDecayTimePtr = parametersPointer->getRawParameterValue(id_EnvDecay);
		envDecayTime = *envDecayTimePtr;

		float* envSustainLevelPtr = parametersPointer->getRawParameterValue(id_EnvSustain);
		envSustainLevel = *envSustainLevelPtr;

		float* envReleaseTimePtr = parametersPointer->getRawParameterValue(id_EnvRelease);
		envReleaseTime = *envReleaseTimePtr;
	}
    
    void updateAllParameters()
    {
		updateParametersPerSample();
        updateParametersEachBlock();
        updateParametersOnStartNote();
    }
    
    void constructModalUnits()
    {
        mModalUnits.clear();
        
        outputScalar = 1.0/numPartialsVar;
        
        for(int i = 0 ; i < numPartialsVar; i++){
            
            ModalUnit* unit = new ModalUnit(i+1);
            unit->setSampleRate(mSampleRate);
            
            mModalUnits.add(unit);
        }
    }

    //=======================================================
private:
	AudioProcessorValueTreeState * parametersPointer;
	MaxiOsc testOsc;
	MaxiOsc noiseOsc;
    
    OwnedArray<ModalUnit> mModalUnits;

	double output = 0.0;
    double mSampleRate = 44100.0f;
	double eventSampleRate = 44100.0f;
	double samplesPerIncrementVar = 1.0f;

    int adsrCounter = 1;
    double adsrValue = 0;
    
    double outputScalar = 1;
    
	double velocityScaled = 0;
    double frequency = 0;
    double newFrequency = 0;
    
	float synthGain = 0;
	float oscillatorGainSmoothed = 0;

	EnvelopeGenerator aDSR;
	EnvelopeGenerator envImpulse;

	float envAttackTime = 0.001f; //is this correct/necessary?
	float envDecayTime = 0.2f;
	float envSustainLevel = 0.0f;
	float envReleaseTime = 0.02f;

	int numPartialsVar = 20;

	float delayTimeVar;
	float delayFeedbackVar;
	float delayDWMixVar;
	float delayPrePostMixVar;

    MaxiDelayline delayLine;
	SVFilter simpleFilter;

};
