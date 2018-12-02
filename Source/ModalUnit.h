/*
  ==============================================================================

    ModalUnit.h
    Created: 18 Nov 2018 3:58:32am
    Author:  Mike

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MaxiOsc.h"
#include "EnvelopeGenerator.h"
#include "SimpleOsc.h"

//==============================================================================
/*
*/
class ModalUnit
{
public:
	ModalUnit(int inPartialNumber)
    :   mPhase(0),
        mPartialNumber(inPartialNumber)
	{
	}
    
    void setSampleRate(double inSampleRate)
    {
        mSampleRate = inSampleRate; 
		mNyquistFreq = (mSampleRate / 2) - 2000;
        testOsc.setSampleRate(mSampleRate);
		simpleSin.setSampleRate(mSampleRate);
		aDSR.setSampleRate(eventSampleRate); //(eventSampleRate);
    }

	void setFrequency(double frequencyIn) {	frequency = frequencyIn*mPartialNumber;	}
	void setFrequencyDelayed(double frequencyIn) { frequencyDelayed = frequencyIn * mPartialNumber; }
	void setEventSampleRate(double eventSampleRateIn) { eventSampleRate = eventSampleRateIn; aDSR.setSampleRate(eventSampleRate);
	}
	void setSamplesPerIncrement(double samplesPerIncrementIn) { samplesPerIncrement = samplesPerIncrementIn; }

	void setVelocity(double velocityIn) { velocity = velocityIn; aDSR.setVelocityValue(velocity); }

	void setPreAttackSeconds(double envPreAttackTimeIn) { preAttackSeconds = envPreAttackTimeIn; aDSR.setPreAttackSeconds(preAttackSeconds); }
	void setPreAttackDecaySeconds(double envPreAttackDecayTimeIn) { preAttackDecaySeconds = envPreAttackDecayTimeIn; aDSR.setPreAttackDecaySeconds(preAttackDecaySeconds); }
	void setAttackSeconds(double envAttackTimeIn) { attackSeconds = envAttackTimeIn; aDSR.setAttackSeconds(attackSeconds); }
	void setDecaySeconds(double envDecayTimeIn) { decaySeconds = envDecayTimeIn; aDSR.setDecaySeconds(decaySeconds); }
	void setSustainPercent(double envSustainLevelIn) { sustainPercent = envSustainLevelIn; aDSR.setSustainPercent(sustainPercent); }
	void setReleaseSeconds(double envReleaseTimeIn) { releaseSeconds = envReleaseTimeIn; aDSR.setReleaseSeconds(releaseSeconds); }
	void setEnterStage(EnvelopeGenerator::EnvelopeStage NewStageIn) { envelopeStage = NewStageIn; aDSR.enterStage(envelopeStage); }

	
	double getSinOutput() 
	{   
		//DBG("NyquistFreq = " << mNyquistFreq );
		if (frequencyDelayed >= (mNyquistFreq)) 
		{ 
			sinOutput = 0.0f;
		}

		else
		{
			sinOutput = testOsc.sinebuf(frequencyDelayed); // allows ~88 oscillators on laptop core
			//sinOutput = testOsc.sinebuf4(frequencyDelayed); // allows ~71 oscillators on laptop core
			//sinOutput = simpleSin.sinewave(frequencyDelayed); // allows ~60 oscillators on laptop core
		}
			
		return sinOutput;
    }

	void setADSRSampleRate()
	{
		
	}

	double getADSROutput()
	{
		//TECHNIQUE FOR REDUCING THE OUTPUT OF THE ADSR TO ONCE EVERY 10 SAMPLES (but need to change envelope internally to add 10 per sample to compensate)
		//
		//adsrValue = aDSR.nextSample();
		//return adsrValue;

		//DBG("eventSampleRate = " << eventSampleRate << " sampleRate = " << mSampleRate << " samplesPerIncrement = " << samplesPerIncrement);
		
		if (adsrCounter >= samplesPerIncrement) {
			adsrValue = aDSR.nextSample();
			adsrCounter = 1;
		}
		else {
			adsrCounter++;
		}
		return adsrValue;
	}
    
	double getOutput()
	{
		output = getSinOutput() * getADSROutput();
		return output;
	}

private:
    
	MaxiOsc testOsc;
	SimpleOsc simpleSin;
	EnvelopeGenerator aDSR;
	double adsrCounter = 0.0;
	double adsrValue = 0.0;
	double eventSampleRate = 44100;
	double samplesPerIncrement = 1.0;
	double velocity = 1.0;

	double preAttackSeconds = 0.0;
	double preAttackDecaySeconds = 0.0;
	double attackSeconds = 0.0;
	double decaySeconds = 0.0;
	double sustainPercent = 0.0;
	double releaseSeconds = 0.0;
	EnvelopeGenerator::EnvelopeStage envelopeStage = EnvelopeGenerator::ENVELOPE_STAGE_ATTACK;

    double mPhase = 0;
    int mPartialNumber = 1;
    double mSampleRate = 44100;
	double mNyquistFreq = 22050;
	double frequency = 330;
	double frequencyDelayed = 400;
	double output = 0.0;
	double sinOutput = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalUnit)
};
