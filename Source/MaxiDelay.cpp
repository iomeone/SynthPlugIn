/*
  ==============================================================================

    MaxiDelay.cpp
    Created: 28 Oct 2018 2:23:06am
    Author:  Mike

  ==============================================================================
*/

#include "MaxiDelay.h"
#include <string.h>
#include <cstdlib>
#include <stdint.h>



//Delay with feedback

MaxiDelayline::MaxiDelayline() {
    
    mDelayBuffer.clear();
}

void MaxiDelayline::setSampleRate(double newSampleRate, double newMaxTime)
{
    sampleRate = newSampleRate;
    
    maxTime = newMaxTime;
    
    mDelayBuffer.clear();
    
    const int bufferSize = sampleRate * maxTime;
    
    for(int i = 0 ; i < bufferSize; i++){
        
        mDelayBuffer.push_back(0.f);
    }
}

double MaxiDelayline::delay(double input, double seconds, double feedback, double prePostMixer, double dryWetMixer) {

	size = seconds * sampleRate; 
	size = jmin(size, (double)mDelayBuffer.size());

	if (phase >= size) { // i believe this means if more time has passed than the time for a delay, then it resets
		phase = 0;
	}
	//output = memory[phase];
	//memory[phase] = (memory[phase] * feedback) + (input*feedback)*0.5;
	//memory[phase] = (memory[phase] * feedback) + (input)*chandiv;
	//phase += 1;
	//return(output);

    
    preOut = mDelayBuffer.at(phase); // Current step in delay buffer = output of Delay block
    
	postOut = preOut * feedback;   // Current delay output with feedback = ouput of Fbk block 
	prePostOut = ((1.0 - prePostMixer) * preOut) + (prePostMixer * postOut);   // Mix of both pre- and post-feedback delay paths = output of XFade (lin) block 

    mDelayBuffer.at(phase) = postOut + input; // feedback back into the start of the delay unit for next cycle
	
	dryOut = (1.0 - dryWetMixer) * input;     //Scaled dry input signal
	wetOut = dryWetMixer * prePostOut;   //Scaled wet delay signal 

	output = dryOut + wetOut; // Blend of (dry) & (pre & post) signal paths 
	
	phase += 1;
	return(output);
	
}

double MaxiDelayline::pureDelay(double input, double seconds) {

	size = seconds * sampleRate;
	size = jmin(size, (double)mDelayBuffer.size());

	if (phase >= size) { // i believe this means if more time has passed than the time for a delay, then it resets
		phase = 0;
	}
	
	output = mDelayBuffer.at(phase);
	mDelayBuffer.at(phase) = input;
	phase += 1;
	return(output);

}

double MaxiDelayline::delayPositional(double input, double seconds, double feedback, int position) {

	size = seconds * sampleRate;
	size = jmin(size, (double)mDelayBuffer.size());

	if (phase >= size) phase = 0;
	if (position >= size) position = 0;
    output = mDelayBuffer.at(phase);
	mDelayBuffer.at(phase) = (mDelayBuffer.at(phase) * feedback) + (input)*chandiv;
	//memory[phase] = (memory[phase] * feedback) + (input*feedback)*chandiv;
	phase += 1;
	return(output);

}
