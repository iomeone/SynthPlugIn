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
		mNyquistFreq = mSampleRate / 2;
        testOsc.setSampleRate(mSampleRate);
		simpleSin.setSampleRate(mSampleRate);
		//aDSR.setSampleRate(mSampleRate);
    }

	void setFrequency(double frequencyIn) {	frequency = frequencyIn*mPartialNumber;	}
//	void setEventSampleRate(double eventSampleRateIn) { eventSampleRate = eventSampleRateIn; }
	void setSamplesPerIncrement(double samplesPerIncrementIn) { samplesPerIncrement = samplesPerIncrementIn; eventSampleRate = mSampleRate / samplesPerIncrement; }

	void setVelocity(double velocityIn) { velocity = velocityIn; aDSR.setVelocityValue(velocity); }

	void setAttackSeconds(double envAttackTimeIn) { attackSeconds = envAttackTimeIn; aDSR.setAttackSeconds(attackSeconds); }
	void setDecaySeconds(double envDecayTimeIn) { decaySeconds = envDecayTimeIn; aDSR.setDecaySeconds(decaySeconds); }
	void setSustainPercent(double envSustainLevelIn) { sustainPercent = envSustainLevelIn; aDSR.setSustainPercent(sustainPercent); }
	void setReleaseSeconds(double envReleaseTimeIn) { releaseSeconds = envReleaseTimeIn; aDSR.setReleaseSeconds(releaseSeconds); }
	void setEnterStage(EnvelopeGenerator::EnvelopeStage NewStageIn) { envelopeStage = NewStageIn; aDSR.enterStage(envelopeStage); }

	
	double getSinOutput() 
	{   
		if (frequency >= 20000) // DOESN'T WORK if (frequency >= mNyquistFreq) -- WHY???
		{ 
			sinOutput = 0.0f;
		}

		else
		{
			sinOutput = testOsc.sinebuf(frequency); // allows ~88 oscillators on laptop core
			//output = testOsc.sinebuf4(frequency); // allows ~71 oscillators on laptop core
			//output = simpleSin.sinewave(frequency); // allows ~60 oscillators on laptop core
		}
			
		return sinOutput;
    }

	double getADSROutput()
	{
		//TECHNIQUE FOR REDUCING THE OUTPUT OF THE ADSR TO ONCE EVERY 10 SAMPLES (but need to change envelope internally to add 10 per sample to compensate)
		aDSR.setSampleRate(eventSampleRate);
		
		DBG("eventSampleRate = " << eventSampleRate << " sampleRate = " << mSampleRate << " samplesPerIncrement = " << samplesPerIncrement);

		if (adsrCounter == 0) {
			adsrValue = aDSR.nextSample();
			adsrCounter = adsrCounter + 1;
		}
		else if (adsrCounter >= samplesPerIncrement) {
			adsrCounter = 0;
		}
		else {
			adsrCounter = adsrCounter + 1;
		}
		return adsrValue;
	}
    
	double getOutput()
	{
		output = getSinOutput();// *getADSROutput();
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
	double output = 0.0;
	double sinOutput = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalUnit)
};
