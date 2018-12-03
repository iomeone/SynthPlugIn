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

	void setSampleRate(double newSampleRate) { sampleRate = newSampleRate; deltaTime = 1 / sampleRate; }
		void setVelocityValue(float inVelocityValue)	{ velocityValue = inVelocityValue; }
		
		void setPreAttackSeconds(float inPreAttackSeconds) { 
			preAttackTimeSeconds = inPreAttackSeconds; 
			preAttackTimeSeconds = jmax(preAttackTimeSeconds, 0.001); 
		}
		void setPreAttackDecaySeconds(float inPreAttackDecaySeconds) {
			preAttackDecayTimeSeconds = inPreAttackDecaySeconds;
			preAttackDecayTimeSeconds = jmax(preAttackDecayTimeSeconds, 0.001);
		}
		void setAttackSeconds(float inAttackSeconds) {
			attackTimeSeconds = inAttackSeconds; 
			attackTimeSeconds = jmax(attackTimeSeconds, 0.001);  
			attackTimeSeconds = attackTimeSeconds /* *velocityValue;*/;
		} 

		void setDecaySeconds(float inDecaySeconds)		{ decayTimeSeconds = inDecaySeconds; decayTimeSeconds = jmax(decayTimeSeconds, 0.001); }
		void setReleaseSeconds(float inReleaseSeconds)	{ releaseTimeSeconds = inReleaseSeconds; releaseTimeSeconds = jmax(releaseTimeSeconds, 0.001); }
		void setSustainPercent(float inSustainPercent)	{ sustainPercent = inSustainPercent; }
		void setAttackOvershoot(float inAttackOvershoot) { attackOvershoot = inAttackOvershoot; }

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
		double deltaTime = 0;
		double velocityValue; // 0.0f - 1.0f
		double currentLevel;
		double stageInitLevel;
		double sampleRate;
		double maxLevel = 0.0;
		double preAttackTimeSeconds = 0.010; // fixed time to switch to attack
		double preAttackDecayTimeSeconds = 0.01; // time to 1/e
		double preAttackSampleLength = 1000;
		double preAttackExponent;
		double attackTimeSeconds = 0.01;
		double decayTimeSeconds = 0.9;
		double releaseTimeSeconds = 0.02;
		double sustainPercent = 0.0;
		double sustainLevel = 0.0;
		double decayExponent;
		double releaseExponent;
		double attackOvershoot = 1.3; //1.0 to 1.4 range roughly
		double attackXMultiplier;
		unsigned long long currentSampleIndex = 0.0;

	};


//Needs way to divide sample rate to reduce CPU load, eg. limit to 5000 calculations per second.

#endif /* defined(__Synthesis__EnvelopeGenerator__) */
