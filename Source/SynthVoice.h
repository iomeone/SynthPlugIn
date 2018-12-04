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

class SynthVoice
:   public SynthesiserVoice,
    public AudioProcessorValueTreeState::Listener
{
public:
	SynthVoice(AudioProcessorValueTreeState* parameters)
		: parametersPointer(parameters)
	{
        //LISTENERS - NEEDED FOR ANYTHING TRIGGERING A FUNCTION

			//otherwise use getRawParameterValue directly

		parametersPointer->addParameterListener(id_SynthGain, this);
        parametersPointer->addParameterListener(id_NumPartials, this);
        parametersPointer->addParameterListener(id_NumUnison, this);
        parametersPointer->addParameterListener(id_DeTuneUnisonHz, this);
        parametersPointer->addParameterListener(id_SamplesPerIncrement, this);

		parametersPointer->addParameterListener(id_EnvPreAttack, this);
		parametersPointer->addParameterListener(id_EnvPreAttackDecay, this);
		parametersPointer->addParameterListener(id_EnvAttack, this);
		parametersPointer->addParameterListener(id_EnvAttackOvershoot, this);
		parametersPointer->addParameterListener(id_EnvDecay, this);
		parametersPointer->addParameterListener(id_EnvSustain, this);
		parametersPointer->addParameterListener(id_EnvRelease, this);

        constructModalUnits();
	}
    
    ~SynthVoice()
    {
		//MUST DESTROY ALL LISTENERS

        parametersPointer->removeParameterListener(id_SynthGain, this);
        parametersPointer->removeParameterListener(id_NumPartials, this);
        parametersPointer->removeParameterListener(id_NumUnison, this);
        parametersPointer->removeParameterListener(id_DeTuneUnisonHz, this);
        parametersPointer->removeParameterListener(id_SamplesPerIncrement, this);

		parametersPointer->removeParameterListener(id_EnvPreAttack, this);
		parametersPointer->removeParameterListener(id_EnvPreAttackDecay, this);
		parametersPointer->removeParameterListener(id_EnvAttack, this);
		parametersPointer->removeParameterListener(id_EnvAttackOvershoot, this);
		parametersPointer->removeParameterListener(id_EnvDecay, this);
		parametersPointer->removeParameterListener(id_EnvSustain, this);
		parametersPointer->removeParameterListener(id_EnvRelease, this);
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
        
        auto delayOnOff = *parametersPointer->getRawParameterValue(id_DelOnOff);
        
        if ((bool)delayOnOff) {
			return delayLine.delay(modalOutput(),
                                   *parametersPointer->getRawParameterValue(id_DelTime),
                                   *parametersPointer->getRawParameterValue(id_DelFeedback),
                                   *parametersPointer->getRawParameterValue(id_DelTime),
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
		//UPDATE MODALUNIT ON START NOTE
		for (int i = 0; i < mModalUnits.size(); i++) {
            ModalUnit* unit = mModalUnits.getUnchecked(i);
            unit->setVelocity(velocity);
			unit->setEnterStage(EnvelopeGenerator::ENVELOPE_STAGE_PREATTACK);
        }

		velocityScaled = velocity;
        newFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        
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
        
       // if (velocity == 0)
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
        
        const float gain = *parametersPointer->getRawParameterValue(id_SynthGain);
        const float preAttack = *parametersPointer->getRawParameterValue(id_EnvPreAttack);
        
		/** render our audio */
		for (int sample = 0; sample < numSamples; ++sample)
        {
            //SAMPLE SMOOTHING
            oscillatorGainSmoothed = oscillatorGainSmoothed - 0.002*(oscillatorGainSmoothed - gain);

            frequency = newFrequency;
            frequencyDelayed = pitchDelay.pureDelay(frequency, preAttack);

            //UPDATE MODALUNIT PER SAMPLE
            for (int i = 0; i < mModalUnits.size(); i++) {

                    ModalUnit* unit = mModalUnits.getUnchecked(i);
                    unit->setFrequency(frequency);
                    unit->setFrequencyDelayed(frequencyDelayed);

                }

            //OUTPUT HERE
            finalOutput = delayOutput()*oscillatorGainSmoothed;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, finalOutput); ///////MAIN OUTPUT HERE
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
        
		samplesPerIncrement = *parametersPointer->getRawParameterValue(id_SamplesPerIncrement);
		eventSampleRate = mSampleRate / samplesPerIncrement;
		
        outputScalar = 1.0/float(numPartials*numUnison);
        
		//UPDATE MODAL UNITS ON CONSTRUCTION
        for(int i = 0 ; i < numPartials; i++){
			for (int j = 0; j < numUnison; j++) {
 
				ModalUnit* unit = new ModalUnit(i + 1, j);
				
                unit->setSampleRate(mSampleRate);
                unit->setEventSampleRate(eventSampleRate);
                unit->setSamplesPerIncrement(samplesPerIncrement);
                unit->setDeTuneUnisonHz(*parametersPointer->getRawParameterValue(id_DeTuneUnisonHz));
				
				unit->setPreAttackSeconds(*parametersPointer->getRawParameterValue(id_EnvPreAttack));
				unit->setPreAttackDecaySeconds(*parametersPointer->getRawParameterValue(id_EnvPreAttackDecay));
				unit->setAttackSeconds(*parametersPointer->getRawParameterValue(id_EnvAttack));
				unit->setAttackOvershoot(*parametersPointer->getRawParameterValue(id_EnvAttackOvershoot));
				unit->setDecaySeconds(*parametersPointer->getRawParameterValue(id_EnvDecay));
				unit->setSustainPercent(*parametersPointer->getRawParameterValue(id_EnvSustain));
				unit->setReleaseSeconds(*parametersPointer->getRawParameterValue(id_EnvRelease));

				mModalUnits.add(unit);
			}
        }
    }
    
    void parameterChanged (const String& parameterID, float newValue) override
    {
		//WHAT TO DO IF KNOBS ARE MOVED

        if((parameterID == id_NumPartials) ||
           (parameterID == id_NumUnison)){
            constructModalUnits();
        }
    
        if(parameterID == id_DeTuneUnisonHz){
            for (int i = 0; i < mModalUnits.size(); i++) {
                ModalUnit* unit = mModalUnits.getUnchecked(i);
                unit->setDeTuneUnisonHz(newValue);
            }
        }
        
        if(parameterID == id_SamplesPerIncrement){
            samplesPerIncrement = newValue;
            eventSampleRate = mSampleRate / samplesPerIncrement;
            for (int i = 0; i < mModalUnits.size(); i++) {
                ModalUnit* unit = mModalUnits.getUnchecked(i);
                unit->setEventSampleRate(eventSampleRate);
                unit->setSamplesPerIncrement(samplesPerIncrement);
            }
        }

		if (parameterID == id_EnvPreAttack) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setPreAttackSeconds(newValue);
			}
		}
		if (parameterID == id_EnvPreAttackDecay) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setPreAttackDecaySeconds(newValue);
			}
		}
		if (parameterID == id_EnvAttack) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setAttackSeconds(newValue);
			}
		}
		if (parameterID == id_EnvAttackOvershoot) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setAttackOvershoot(newValue);
			}
		}
		if (parameterID == id_EnvDecay) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setDecaySeconds(newValue);
			}
		}
		if (parameterID == id_EnvSustain) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setSustainPercent(newValue);
			}
		}
		if (parameterID == id_EnvRelease) {
			for (int i = 0; i < mModalUnits.size(); i++) {
				ModalUnit* unit = mModalUnits.getUnchecked(i);
				unit->setReleaseSeconds(newValue);
			}
		}


    }

    //=======================================================
private:
    
    CriticalSection lock;
    
	AudioProcessorValueTreeState * parametersPointer;
	MaxiOsc testOsc;
	MaxiOsc noiseOsc;
    
    OwnedArray<ModalUnit> mModalUnits;

	double output = 0.0;
    double mSampleRate = 44100.0f;
	double eventSampleRate = 44100.0f;
	double samplesPerIncrement = 1.0f;

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
