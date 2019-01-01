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

private:
	double frequency;
    double maxTime = 2.0;
	int phase = 0;
	double startphase;
	double endphase;
	double output;
	double preOut;
	double postOut;
	double prePostOut;
	double dryOut;
	double wetOut;
	double size;
	double sampleRate;
	float chandiv = 1; //This used to be important for dealing with multichannel playback
    
    std::vector<float> mDelayBuffer;

public:
	MaxiDelayline();
	double delay(double input, double seconds, double feedback, double prePostMixer, double dryWetMixer);
	void renderNextBlock(AudioBuffer<float>& bufferIn, float delayTimeIn, float delayFeedbackIn, float delayPrePostMixIn, float delayDWMixIn);
	void renderNextBlockMono(AudioBuffer<float>& bufferIn, float delayTimeIn, float delayFeedbackIn, float delayPrePostMixIn, float delayDWMixIn);
	double pureDelay(double input, double seconds);
	double delayPositional(double input, double seconds, double feedback, int position);
    void setSampleRate(double newSampleRate, double newMaxTime); // { sampleRate = newSampleRate; }
	
};
