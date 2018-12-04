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
		
		currentLevel = stageInitLevel * exp(log(currentDeltaIndex) * negativePreAttackExponent);
		currentDeltaIndex = currentDeltaIndex + deltaTime;

		if (currentDeltaIndex >= preAttackTimeSecondsPlusOne) {
			enterStage(ENVELOPE_STAGE_ATTACK);
		}
	}
	else if (currentStage == ENVELOPE_STAGE_ATTACK) {
	
		currentLevel = stageInitLevel + (1 - exp(attackXMultiplier*currentDeltaIndex))*attackOvershoot*velocityValue;
		currentDeltaIndex = currentDeltaIndex + deltaTime;

		if (currentLevel >= maxLevel) {
			enterStage(ENVELOPE_STAGE_DECAY);
		}
	}

	else if (currentStage == ENVELOPE_STAGE_DECAY) {
		
		currentLevel = maxLevel * exp(log(currentDeltaIndex) * negativeDecayExponent);
		currentDeltaIndex = currentDeltaIndex + deltaTime;
		
		if (currentLevel <= (sustainLevelPlusIncrement)) {
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
			
		currentLevel = stageInitLevel * exp(log(currentDeltaIndex) * negativeReleaseExponent);
		currentDeltaIndex = currentDeltaIndex + deltaTime;
		
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
			currentDeltaIndex = 1.0f;
			stageInitLevel = currentLevel;
			break;

		case ENVELOPE_STAGE_ATTACK: 
			currentDeltaIndex = 0.0f;
			stageInitLevel = currentLevel;
			maxLevel = jmin(1.0f, stageInitLevel + velocityValue);
			attackXMultiplier = (log(1 - (1 / attackOvershoot))) / (attackTimeSeconds*velocityValue);
			break;

        case ENVELOPE_STAGE_DECAY:
			currentDeltaIndex = 1.0f;
			sustainLevel = sustainPercent * maxLevel;
			sustainLevelPlusIncrement = sustainLevel + 0.000001;
			break;

        case ENVELOPE_STAGE_SUSTAIN:
			currentLevel = sustainLevel;
            break;

        case ENVELOPE_STAGE_RELEASE:
			currentDeltaIndex = 1.0f;
			stageInitLevel = currentLevel;
			break;

        default:
            break;
    }
}



