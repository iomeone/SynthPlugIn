/*
  ==============================================================================

    GlobalFunctions.h
    Created: 27 Nov 2018 12:32:27am
    Author:  Mike

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

inline float denormalize(float inValue){
    
    float absValue = fabs(inValue);
    
    if(absValue < 1e-15){
        return 0.f;
    } else {
        return inValue;
    }
}

inline float getRandomNumber(float inMin, float inMax)
{
    // 0.0 - 1.0
    float value = rand() / (float)RAND_MAX;
    value = denormalize(value);
    
    return jmap(value, 0.0f, 1.0f, inMin, inMax);
}

inline Array<float> getSineTable(float inTableSize)
{
    Array<float> table;
    
    float period = 1.0f/inTableSize;
    
    for(int i = 0; i < inTableSize; i++){
        float value = sinf(MathConstants<double>::twoPi * i * period);
        table.add(value);
    }
    
    return table;
}

//Add convert_TtoS and convert_StoT, convert_mStoT, convert_TtomS
