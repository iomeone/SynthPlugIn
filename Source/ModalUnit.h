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
class ModalUnit    : public MaxiOsc, public EnvelopeGenerator
{
public:
	ModalUnit()
	{
		
	}

	void setPerSample(double frequency)
	{
		frequencyOsc = frequency;
	}

	double getOutput(double frequency)
	{
		output = testOsc.sinewave(frequency);
		return output;
	}
	
	
private:
	MaxiOsc testOsc;
	//EnvelopeGenerator testADSR;
	double frequencyOsc = 440;
	double output = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalUnit)
};
