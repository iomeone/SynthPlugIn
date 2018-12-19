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
#include "LadderFilter.h"
#include "NoiseGenerator.h"
#include "ModalUnit.h"

#include "UsedParameters.h"
#include "GlobalFunctions.h"

class SynthVoice
:   public SynthesiserVoice,
    public AudioProcessorValueTreeState::Listener
{
public:
	SynthVoice(AudioProcessorValueTreeState* parameters)
		: parametersPointer(parameters)
	{
        parametersPointer->addParameterListener(id_SynthGain, this);
        parametersPointer->addParameterListener(id_NumPartials, this);
        parametersPointer->addParameterListener(id_NumUnison, this);
        parametersPointer->addParameterListener(id_DeTuneUnisonHz, this);
        parametersPointer->addParameterListener(id_SamplesPerIncrement, this);
        parametersPointer->addParameterListener(id_DelOnOff, this);
        
        constructModalUnits();
	}
    
    ~SynthVoice()
    {
        parametersPointer->removeParameterListener(id_SynthGain, this);
        parametersPointer->removeParameterListener(id_NumPartials, this);
        parametersPointer->removeParameterListener(id_NumUnison, this);
        parametersPointer->removeParameterListener(id_DeTuneUnisonHz, this);
        parametersPointer->removeParameterListener(id_SamplesPerIncrement, this);
        parametersPointer->removeParameterListener(id_DelOnOff, this);
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
        
		envImpulse.setSampleRate(mSampleRate);
        delayLine.setSampleRate(mSampleRate, 2.0);
		testOsc.setSampleRate(mSampleRate);
		noiseOsc.setSampleRate(mSampleRate);
        simpleFilter.setSampleRate(mSampleRate);
		pitchDelay.setSampleRate(mSampleRate, 2.0);
        
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

	double delayOutput()
	{
        
        auto delayOnOff =
        *parametersPointer->getRawParameterValue(id_DelOnOff);
        
        if ((bool)delayOnOff) {
            
//            double MaxiDelayline::delay(double input, double seconds, double feedback, double prePostMixer, double dryWetMixer)
            
            
			return delayLine.delay(modalOutput(),
                                   *parametersPointer->getRawParameterValue(id_DelTime),
                                   *parametersPointer->getRawParameterValue(id_DelFeedback),
                                   *parametersPointer->getRawParameterValue(id_DelPrePostMix),
                                   *parametersPointer->getRawParameterValue(id_DelDWMix));
		}
		else {
			return modalOutput();
		}
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
        mNoteOnActive = true;
        
		//UPDATE MODALUNIT ON START NOTE
		for (int i = 0; i < mModalUnits.size(); i++) {
            ModalUnit* unit = mModalUnits.getUnchecked(i);
            unit->setVelocity(velocity);
            unit->setPreAttackSeconds(*parametersPointer->getRawParameterValue(id_EnvPreAttack));
            unit->setPreAttackDecaySeconds(*parametersPointer->getRawParameterValue(id_EnvPreAttackDecay));
            unit->setAttackSeconds(*parametersPointer->getRawParameterValue(id_EnvAttack));
            unit->setDecaySeconds(*parametersPointer->getRawParameterValue(id_EnvDecay));
            unit->setSustainPercent(*parametersPointer->getRawParameterValue(id_EnvSustain));
            unit->setReleaseSeconds(*parametersPointer->getRawParameterValue(id_EnvRelease));
            unit->setEnterStage(EnvelopeGenerator::ENVELOPE_STAGE_PREATTACK);
        }

        const float rand = getRandomNumber(-1.0, 1.0);
        
		velocityScaled = velocity;
        newFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber) + (50 * rand);
        
        if(newFrequency < 20.f){
            newFrequency = 20.f;
        }
        
		simpleFilter.setFilterType(2);
		simpleFilter.setCutoffFreq(300);
		simpleFilter.setQ(1256); // add function setModalQ needs to take decay time in seconds, and note pitch as inputs and multiple by 2 pi
				
		envImpulse.setVelocityValue(velocity);
		envImpulse.setAttackSeconds(0.01);
		envImpulse.setDecaySeconds(0.05);
		envImpulse.setSustainPercent(0.0);
		envImpulse.setReleaseSeconds(0.01);
        envImpulse.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
	}
    
    //=======================================================
    
    void stopNote (float velocity, bool allowTailOff) override
    {
		//UPDATE MODALUNIT ON STOP NOTE
		for (int i = 0; i < mModalUnits.size(); i++) {
			ModalUnit* unit = mModalUnits.getUnchecked(i);
			unit->setEnterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}

        allowTailOff = false;
        
        mNoteOnActive = false;
        
        mNoteOnTimeTick = 1;
        
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
        const ScopedLock renderLock (lock);
        
        const float gain =
        *parametersPointer->getRawParameterValue(id_SynthGain);
        
        const float preAttack =
        *parametersPointer->getRawParameterValue(id_EnvPreAttack);
        
		/** render our audio */
		for (int sample = 0; sample < numSamples; ++sample)
        {
            //SAMPLE SMOOTHING
            oscillatorGainSmoothed = oscillatorGainSmoothed -
            0.002*(oscillatorGainSmoothed - gain);

            frequency = newFrequency;
            frequencyDelayed =
            pitchDelay.pureDelay(frequency,
                                 preAttack);

            //UPDATE MODALUNIT PER SAMPLE
            for (int i = 0; i < mModalUnits.size(); i++) {

                ModalUnit* unit = mModalUnits.getUnchecked(i);
                unit->setFrequency(frequency);
                unit->setFrequencyDelayed(frequencyDelayed);
                unit->setNoteOnTimeTick(mNoteOnTimeTick);
            }

            //OUTPUT HERE
            finalOutput = delayOutput()*oscillatorGainSmoothed;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, finalOutput); ///////MAIN OUTPUT HERE
            }
            
            if(mNoteOnActive){
                mNoteOnTimeTick = mNoteOnTimeTick + 1;
                
                if(mNoteOnTimeTick >= samplesPerIncrementVar){
                    mNoteOnTimeTick = 1;
                }
            }
            
            ++startSample;
		}
	
    }
    
    void constructModalUnits()
    {
        const ScopedLock constructLock (lock);
        
        const int numPartials =
        *parametersPointer->getRawParameterValue(id_NumPartials);
        
        const int numUnison =
        *parametersPointer->getRawParameterValue(id_NumUnison);
        
        mModalUnits.clear();
        
        outputScalar = 1.0/float(numPartials*numUnison);
        
        for(int i = 0 ; i < numPartials; i++){
			for (int j = 0; j < numUnison; j++) {
 
				ModalUnit* unit = new ModalUnit(i + 1, j);
				
                unit->setSampleRate(mSampleRate);
                unit->setEventSampleRate(eventSampleRate);
                unit->setSamplesPerIncrement(samplesPerIncrementVar);
                unit->setDeTuneUnisonHz(*parametersPointer->getRawParameterValue(id_DeTuneUnisonHz));
				mModalUnits.add(unit);
			}
        }
    }
    
    void parameterChanged (const String& parameterID, float newValue) override
    {
        if((parameterID == id_NumPartials) ||
           (parameterID == id_NumUnison)){
            
            constructModalUnits();
        }
    
        else if(parameterID == id_DeTuneUnisonHz){
            
            for (int i = 0; i < mModalUnits.size(); i++) {
                
                ModalUnit* unit = mModalUnits.getUnchecked(i);
                unit->setDeTuneUnisonHz(newValue);
            }
        }
        
        else if(parameterID == id_SamplesPerIncrement){
            
            samplesPerIncrementVar = newValue;
            eventSampleRate = mSampleRate / samplesPerIncrementVar;
            
            for (int i = 0; i < mModalUnits.size(); i++) {
                
                ModalUnit* unit = mModalUnits.getUnchecked(i);
                unit->setEventSampleRate(eventSampleRate);
                unit->setSamplesPerIncrement(samplesPerIncrementVar);
            }
        }
        
        else if(parameterID == id_DelOnOff){
            DBG("delay on off: " << (int)newValue);
        }
    }

    //=======================================================
private:
    
    CriticalSection lock;
    
	AudioProcessorValueTreeState * parametersPointer;
	MaxiOsc testOsc;
	MaxiOsc noiseOsc;
    
    OwnedArray<ModalUnit> mModalUnits;
    
    bool mNoteOnActive = false;
    int mNoteOnTimeTick = 1;

	double output = 0.0;
    double mSampleRate = 44100.0f;
	double eventSampleRate = 44100.0f;
	double samplesPerIncrementVar = 1.0f;

	MaxiDelayline pitchDelay;
	double frequencyDelayed = 400;
    
    double outputScalar = 1;
    
	double velocityScaled = 0;
    double frequency = 0;
    double newFrequency = 0;
    
	float oscillatorGainSmoothed = 0;
    
    EnvelopeGenerator envImpulse;

	double finalOutput = 0.0;
 
    MaxiDelayline delayLine;
	SVFilter simpleFilter;

};
