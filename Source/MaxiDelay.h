/*
  ==============================================================================

    MaxiDelay.h
    Created: 28 Oct 2018 2:23:06am
    Author:  Mike

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class MaxiDelayline {
	double frequency;
    double maxTime = 2.0;
	int phase = 0;
	double startphase;
	double endphase;
	double output;
	double preOut;
	double postOut;
	//double prePostMixer = 0.0;
	double prePostOut;
	//double dryWetMixer = 1.0;
	double dryOut;
	double wetOut;
	double size;
	double sampleRate;
	// double memory[96000]; //sets the maximum delay time possible ie. 96000 at 48kHz sampling gives max 2 second delay
	float chandiv = 1; //This used to be important for dealing with multichannel playback
    
    std::vector<float> mDelayBuffer;

public:
	MaxiDelayline();
	double delay(double input, double seconds, double feedback, double prePostMixer, double dryWetMixer);
	double dl(double input, int size, double feedback, int position);
    void setSampleRate(double newSampleRate, double newMaxTime); // { sampleRate = newSampleRate; }
	
};
