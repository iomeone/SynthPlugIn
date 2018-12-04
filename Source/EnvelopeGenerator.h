//
//  EnvelopeGenerator.h
//  Synthesis
//
//  Created by Martin on 08.04.14.
//
//
#pragma once

#ifndef __Synthesis__EnvelopeGenerator__
#define __Synthesis__EnvelopeGenerator__

#include <cmath>
#include "JuceHeader.h"

class EnvelopeGenerator {

public:
	enum EnvelopeStage {
		ENVELOPE_STAGE_OFF = 0,
		ENVELOPE_STAGE_PREATTACK,
		ENVELOPE_STAGE_ATTACK,
		ENVELOPE_STAGE_DECAY,
		ENVELOPE_STAGE_SUSTAIN,
		ENVELOPE_STAGE_RELEASE,
		kNumEnvelopeStages
	};

	void enterStage(EnvelopeStage newStage);
	double nextSample();

	inline EnvelopeStage getCurrentStage() const { return currentStage; };
	inline double getCurrentSampleIndex() const { return currentSampleIndex; };
	inline double getPreAttackSampleLength() const {return preAttackSampleLength;};
	inline double getSampleRate() const { return sampleRate; };

	void setSampleRate(double newSampleRate) { 
		sampleRate = newSampleRate; 
		deltaTime = 1 / sampleRate; 
	}
		
	void setVelocityValue(float inVelocityValue)	{ 
			velocityValue = inVelocityValue;
		}
		
		void setPreAttackSeconds(float inPreAttackSeconds) { 
			preAttackTimeSeconds = inPreAttackSeconds; 
			preAttackTimeSeconds = jmax(preAttackTimeSeconds, 0.001f);
			preAttackSampleLength = preAttackTimeSeconds * sampleRate;
			preAttackTimeSecondsPlusOne = preAttackTimeSeconds + 1;
		}
		void setPreAttackDecaySeconds(float inPreAttackDecaySeconds) {
			preAttackDecayTimeSeconds = inPreAttackDecaySeconds;
			preAttackDecayTimeSeconds = jmax(preAttackDecayTimeSeconds, 0.001f);
			negativePreAttackExponent = -1 / log(preAttackDecayTimeSeconds + 1);
		}

		void setAttackSeconds(float inAttackSeconds) {
			attackTimeSeconds = inAttackSeconds; 
			attackTimeSeconds = jmax(attackTimeSeconds, 0.001f);
		} 

		void setDecaySeconds(float inDecaySeconds)		{
			decayTimeSeconds = inDecaySeconds; 
			decayTimeSeconds = jmax(decayTimeSeconds, 0.001f); 
			negativeDecayExponent = -1 / log(decayTimeSeconds + 1);
		}
		void setReleaseSeconds(float inReleaseSeconds)	{ 
			releaseTimeSeconds = inReleaseSeconds; 
			releaseTimeSeconds = jmax(releaseTimeSeconds, 0.001f); 
			negativeReleaseExponent = -1 / log(releaseTimeSeconds + 1);
		}
		void setSustainPercent(float inSustainPercent)	{ sustainPercent = inSustainPercent; }
		
		void setAttackOvershoot(float inAttackOvershoot) { 
			attackOvershoot = jmax(inAttackOvershoot, 1.0001f); 
		}

		
		EnvelopeGenerator() : //constructor?
			currentStage(ENVELOPE_STAGE_OFF),
			currentLevel(0.0),
			sampleRate(44100.0),
			currentSampleIndex(0) 
		{
		};


	private:
		EnvelopeStage currentStage;
		//int samplesPerIncrement = 1;
		float deltaTime = 0;
		float velocityValue; // 0.0f - 1.0f
		float currentLevel;
		float stageInitLevel;
		float sampleRate;
		float maxLevel = 0.0;
		float preAttackTimeSeconds = 0.010; // fixed time to switch to attack
		float preAttackTimeSecondsPlusOne = 1.0f;
		float preAttackDecayTimeSeconds = 0.01; // time to 1/e
		float preAttackSampleLength = 1000;
		float negativePreAttackExponent;
		float attackTimeSeconds = 0.01;
		float decayTimeSeconds = 0.9;
		float releaseTimeSeconds = 0.02;
		float sustainPercent = 0.0;
		float sustainLevel = 0.0;
		float sustainLevelPlusIncrement = 0.0f;
		float negativeDecayExponent = 1.0f;
		float negativeReleaseExponent;
		float attackOvershoot = 1.3; //1.0 to 1.4 range roughly
		float attackXMultiplier;
		unsigned long long currentSampleIndex = 0.0;
		double currentDeltaIndex = 1.0;

	};


//Needs way to divide sample rate to reduce CPU load, eg. limit to 5000 calculations per second.

#endif /* defined(__Synthesis__EnvelopeGenerator__) */
