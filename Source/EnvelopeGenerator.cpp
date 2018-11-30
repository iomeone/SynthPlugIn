//
//  EnvelopeGenerator.cpp
//  Synthesis
//
//  Created by Martin on 08.04.14.
//
//

#include "EnvelopeGenerator.h"

double EnvelopeGenerator::nextSample() {
    
	if (currentStage == ENVELOPE_STAGE_PREATTACK) {
		
		currentLevel = 0;// stageInitLevel / pow((currentSampleIndex / sampleRate) + 1.0, preAttackExponent);
		currentSampleIndex++;

		//DBG("sample index = " << currentSampleIndex << " level = " << currentLevel);

		if (currentSampleIndex >= (preAttackTimeSeconds * sampleRate)) {
			enterStage(ENVELOPE_STAGE_ATTACK);
		}
	}
	else if (currentStage == ENVELOPE_STAGE_ATTACK) {
	
		currentLevel = stageInitLevel + (1 - exp(attackXMultiplier*(currentSampleIndex/sampleRate)))*attackOvershoot;
		currentSampleIndex++;
		
		if (currentLevel >= maxLevel) {
			enterStage(ENVELOPE_STAGE_DECAY);
		}
	}

	else if (currentStage == ENVELOPE_STAGE_DECAY) {
		
		currentLevel = maxLevel / pow((currentSampleIndex/sampleRate)+1.0, decayExponent);
		currentSampleIndex++;
		
		if (currentLevel <= (sustainLevel + 0.000001)) {
			currentLevel = sustainLevel;
			enterStage(ENVELOPE_STAGE_SUSTAIN);
		}	
	}

	else if (currentStage == ENVELOPE_STAGE_SUSTAIN) {

		if (currentLevel == 0.0) {
			enterStage(ENVELOPE_STAGE_OFF);
		}
	}

	else if (currentStage == ENVELOPE_STAGE_RELEASE) {
			
		currentLevel = stageInitLevel / pow((currentSampleIndex/sampleRate)+1.0, releaseExponent);
		currentSampleIndex++;

		if (currentLevel <= 0.0000001) {
			currentLevel = 0.0; 
			enterStage(ENVELOPE_STAGE_OFF);
		}
	}
	else {}
	

    return currentLevel;
}


void EnvelopeGenerator::enterStage(EnvelopeStage newStage) {
   currentSampleIndex = 0;
   currentStage = newStage;

    switch (newStage) {

        case ENVELOPE_STAGE_OFF:
			currentLevel = 0.0;
            break;

		case ENVELOPE_STAGE_PREATTACK:
			stageInitLevel = currentLevel;
			preAttackExponent = 1 / log(preAttackDecayTimeSeconds + 1);
			break;

		case ENVELOPE_STAGE_ATTACK: 
			stageInitLevel = currentLevel;
			maxLevel = jmin(1.0, stageInitLevel + velocityValue);
			attackXMultiplier = (log(1 - (1 / attackOvershoot))) / attackTimeSeconds;
            break;

        case ENVELOPE_STAGE_DECAY:
			sustainLevel = sustainPercent * maxLevel;
			decayExponent = 1 / log(decayTimeSeconds+1);
			break;

        case ENVELOPE_STAGE_SUSTAIN:
			currentLevel = sustainLevel;
            break;

        case ENVELOPE_STAGE_RELEASE:
			stageInitLevel = currentLevel;
			releaseExponent =  1 / log(releaseTimeSeconds + 1); 
			break;

        default:
            break;
    }
}



