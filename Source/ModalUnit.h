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
    }

	void setPerSample(double frequencyIn)
	{
		frequency = frequencyIn*mPartialNumber;
	}

	void setPerBlock()
	{

	}
	void setOnNoteOn()
	{

	}

	double getOutput() 
	{   
		if (frequency >= 22000) // DOESN'T WORK if (frequency >= mNyquistFreq) -- WHY???
		{ 
			output = 0.0f;
		}

		else
		{
			output = testOsc.sinebuf(frequency); // allows ~88 oscillators on laptop core
			//output = testOsc.sinebuf4(frequency); // allows ~71 oscillators on laptop core
			//output = simpleSin.sinewave(frequency); // allows ~60 oscillators on laptop core
		}
			
		return output;
    }
    

private:
    
	MaxiOsc testOsc;
	SimpleOsc simpleSin;
	//EnvelopeGenerator testADSR;
    
    double mPhase;
    int mPartialNumber;
    double mSampleRate;
	double mNyquistFreq;
	double frequency = 330;
	double output = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalUnit)
};
