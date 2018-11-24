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
    
//    memset(memory, 0, 96000 * sizeof(double));
    
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


//To allow feedback to work without destroying your sinewaves on random notes, you need a way to input the notepitch (noteOn)
//Then you take 1/Hz to get period length
//delay time / period length = odd number would indicate destuctive interference.
//take delay / period and round it off. if odd, subtract/add one. Then multiply by period again to get delay time.
//problem is it might create all sorts of discontinuities because can't just keep creating new delay time changes - throws out data I think.

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

	//output = memory[phase];
    
    preOut = mDelayBuffer.at(phase);
	//preOut = memory[phase];     // Current step in delay buffer = output of Delay block
    
	postOut = preOut * feedback;   // Current delay output with feedback = ouput of Fbk block 
	prePostOut = ((1.0 - prePostMixer) * preOut) + (prePostMixer * postOut);   // Mix of both pre- and post-feedback delay paths = output of XFade (lin) block 

    mDelayBuffer.at(phase) = postOut + input; // feedback back into the start of the delay unit for next cycle
	//memory[phase] = postOut + input; // feedback back into the start of the delay unit for next cycle

	dryOut = (1.0 - dryWetMixer) * input;     //Scaled dry input signal
	wetOut = dryWetMixer * prePostOut;   //Scaled wet delay signal 

	output = dryOut + wetOut; // Blend of (dry) & (pre & post) signal paths 
	
	phase += 1;
	return(output);
	
}

double MaxiDelayline::dl(double input, int size, double feedback, int position) {
	if (phase >= size) phase = 0;
	if (position >= size) position = 0;
    output = mDelayBuffer.at(phase);
	mDelayBuffer.at(phase) = (mDelayBuffer.at(phase) * feedback) + (input)*chandiv;
	//memory[phase] = (memory[phase] * feedback) + (input*feedback)*chandiv;
	phase += 1;
	return(output);

}
