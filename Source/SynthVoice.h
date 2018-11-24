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
        
        outputScalar = 1.0/numOscillators;
        
        for(int i = 0; i < numOscillators; i ++){
            mOscillators.add(new MaxiOsc());
        }
	}

    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }
    
	//=======================================================

	void setCurrentPlaybackSampleRate(double newRate) override
	{
		SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);

		aDSR.setSampleRate(newRate);
		envImpulse.setSampleRate(newRate);
        delayLine.setSampleRate(newRate, 2.0);
		testOsc.setSampleRate(newRate);
		noiseOsc.setSampleRate(newRate);
        simpleFilter.setSampleRate(newRate);
        
        for(int i = 0; i < mOscillators.size(); i ++){
            
            MaxiOsc* osc = mOscillators.getUnchecked(i);
            osc->setSampleRate(newRate);
            
        }
	}
    //=======================================================
    
    double oscOutput()
    {
		// return testOsc.sinewave(frequency);
        
        double output = 0.0;
        
        for(int i = 0; i < mOscillators.size(); i ++){
            
            MaxiOsc* osc = mOscillators.getUnchecked(i);
            output = output + (osc->sinewave((i+1)*frequency) /(i+1));
            
        }
        
        output = output * outputScalar;   	//ie. output = output / mOscillators.size();
        
        return output;
    }
	
	//=======================================================

	double modalUnitOutput()
	{
		testModalUnit.setPerSample(frequency);
		return testModalUnit.getOutput();
	}

	//=======================================================

	double aDSROutput()
	{
        return aDSR.nextSample();
        
		/* TECHNIQUE FOR REDUCING THE OUTPUT OF THE ADSR TO ONCE EVERY 10 SAMPLES (but need to change envelope internally to add 10 per sample to compensate)
        if(adsrCounter == 0){
            adsrValue = aDSR.nextSample();
        }
        else if(adsrCounter >= 10){
            adsrCounter = 0;
        }
        else {
            adsrCounter = adsrCounter + 1;
        }
        return adsrValue;*/
	}

	double delayOutput()
	{
        return delayLine.delay(aDSROutput()*oscOutput(), 0.5, 0.4, 0.0, 0.2);
	}

	double mixedSignal()
	{
		return delayOutput()*0.3;
		//return aDSROutput()*oscOutput()*0.3;
	}

	double filterOutput()
	{
		return simpleFilter.processAudioSample(noiseImpulse(), 1);
	}
	//=======================================================

	double noiseImpulse()
	{
		return (envImpulse.nextSample() * noiseOsc.noise()) * 0.3;
	}
	
    //=======================================================
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
		updateParametersOnStartNote();

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


		/** render our audio */
		for (int sample = 0; sample < numSamples; ++sample)
		{
			/** sample smoothing */
			oscillatorGainSmoothed = oscillatorGainSmoothed - 0.002*(oscillatorGainSmoothed - synthGain);
            frequency = newFrequency;

			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
                outputBuffer.addSample(channel, startSample, delayOutput()*oscillatorGainSmoothed); ///////MAIN OUTPUT HERE
			}
			++startSample;
		}
	
    }
	
	void updateParametersEachBlock()
	{
		float* inGain = parametersPointer->getRawParameterValue(id_SynthGain);
		//        oscillatorGain = *parametersPointer->getRawParameterValue(id_SynthGain);

		if (synthGain != *inGain) {
			synthGain = *inGain;
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

    //=======================================================
private:
	AudioProcessorValueTreeState * parametersPointer;

	ModalUnit testModalUnit;

	MaxiOsc testOsc;
	MaxiOsc noiseOsc;
    
    OwnedArray<MaxiOsc> mOscillators;
    const int numOscillators = 50; // HOW TO ADAPT OR PROTECT AGAINST NOTES BEING MADE ABOVE 1/2 SAMPLE RATE HZ?? eg. set to 100 and glitches
        
    int adsrCounter = 0;
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

    MaxiDelayline delayLine;
	SVFilter simpleFilter;

};
