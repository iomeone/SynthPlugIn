/*
  ==============================================================================

    MaxiOsc.h
    Created: 26 Oct 2018 11:12:06pm
    Author:  Mike

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


using namespace std;
#ifndef PI
#define PI  3.1415926535897932384626433832795
#endif
#define TWOPI 6.283185307179586476925286766559

class maxiSettings {
public:
	static int sampleRate;
	static int channels;
	static int bufferSize;
	static void setup(int initSampleRate, int initChannels, int initBufferSize) {
		maxiSettings::sampleRate = initSampleRate;
		maxiSettings::channels = initChannels;
		maxiSettings::bufferSize = initBufferSize;
	}
};

class MaxiOsc {

	double frequency;
	double phase;
	double startphase;
	double endphase;
	double output;
	double tri;
	double sampleRate;


public:
	MaxiOsc();
	double sinewave(double frequency);
	double coswave(double frequency);
	double phasor(double frequency);
	double phasor(double frequency, double startphase, double endphase);
	double saw(double frequency);
	double triangle(double frequency);
	double square(double frequency);
	double pulse(double frequency, double duty);
	double noise();
	double sinebuf(double frequency);
	double sinebuf4(double frequency);
	double sawn(double frequency);
	double rect(double frequency, double duty = 0.5);
	void phaseReset(double phaseIn);
	void setSampleRate(double newSampleRate) { sampleRate = newSampleRate; }

private:
	//This used to be important for dealing with multichannel playback
	float chandiv = 1;
};
