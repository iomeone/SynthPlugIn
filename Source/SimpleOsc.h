/*
  ==============================================================================

    SimpleOsc.h
    Created: 26 Nov 2018 10:11:28pm
    Author:  Mike

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SimpleOsc
{
public:
	SimpleOsc() : mPhase(0)
	{
	}

	double sinewave(double frequency)
	{
		mPhase = mPhase + (frequency * mSamplePeriod);

		if (mPhase > 1.f) {
			mPhase = mPhase - 1.f;
		}

		return sinf(mPhase * MathConstants<double>::twoPi);
	}

	void reset()
	{
		mPhase = 0.0;
	}

	void setSampleRate(double newSampleRate) 
	{ 
		mSampleRate = newSampleRate; 
		mSamplePeriod = 1.0 / mSampleRate;
	}


private:
	double mPhase;
	double mSampleRate;
	double mSamplePeriod;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleOsc)
};