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
        mSamplePeriod = 1.0/mSampleRate;
        
        testOsc.setSampleRate(mSampleRate);
    }

	void setPerSample(double frequency)
	{
		frequencyOsc = frequency;
	}

	double getOutput(double frequency)
	{   
		return sinewave(mPartialNumber*frequency);
    }
    
    double sinewave(double frequency)
    {
        mPhase = mPhase + (frequency * mSamplePeriod);

        if(mPhase > 1.f){
            mPhase = mPhase - 1.f;
        }
        
        return sinf(mPhase * MathConstants<double>::twoPi);
    }
	
private:
    
	MaxiOsc testOsc;
	//EnvelopeGenerator testADSR;
    
    double mPhase;
    int mPartialNumber;
    double mSamplePeriod;
    double mSampleRate;
	double frequencyOsc = 440;
	double output = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalUnit)
};
