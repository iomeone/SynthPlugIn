/*
  ==============================================================================

    GUIElements.h
    Created: 24 Oct 2018 1:19:53am
    Author:  Mike

  ==============================================================================
*/

#include <iostream>

#include "LabeledComponent.h"

#pragma once


//NUM PARTIALS
void AudioPlugInAudioProcessorEditor::createNumPartialsGroup()
{
	Array<Component*> numPartialsGroupSliders;

	//# PARTIALS
	LabeledComponent* numPartialsSlider = new LabeledComponent(kLabeledComponentStyle_Slider,
                                                               processor.parameters,
                                                               id_NumPartials);
	numPartialsSlider->setInterval(1.0f);
	numPartialsSlider->setNumDecimalPlacesToDisplay(0);
	//numPartialsSlider->setSkewFactorFromMidPoint(0.5);
	numPartialsGroupSliders.add(numPartialsSlider);

	LabeledGroup* numPartialsGroup = new LabeledGroup("# Partials", numPartialsGroupSliders);
	mLabeledGroups.add(numPartialsGroup);
	addAndMakeVisible(numPartialsGroup);
	//gainGroup->setLookAndFeel(&labeledGroupLookAndFeel);
}

//GAIN
void AudioPlugInAudioProcessorEditor::createGainGroup()
{
	Array<Component*> gainGroupSliders;

	//GAIN SLIDER
	LabeledSlider* gainLevelSlider = new LabeledSlider(processor.parameters, id_SynthGain);
    gainLevelSlider->setInterval(0.05);
	gainLevelSlider->setNumDecimalPlacesToDisplay(3); 
	gainLevelSlider->setSkewFactorFromMidPoint(0.7);
    gainGroupSliders.add(gainLevelSlider);

	LabeledGroup* gainGroup = new LabeledGroup("Gain", gainGroupSliders);
	mLabeledGroups.add(gainGroup);
	addAndMakeVisible(gainGroup);
	//gainGroup->setLookAndFeel(&labeledGroupLookAndFeel);
}

//ENVELOPE
void AudioPlugInAudioProcessorEditor::createEnvelopeGroup()
{
	Array<Component*> envelopeGroupSliders;
    
	//ATTACK SLIDER
	LabeledSlider* envAttackSlider = new LabeledSlider(processor.parameters, id_EnvAttack);
	envAttackSlider->setInterval(0.001);
	envAttackSlider->setNumDecimalPlacesToDisplay(4);
	envAttackSlider->setSkewFactorFromMidPoint(0.01); 
	envelopeGroupSliders.add(envAttackSlider);

	//DECAY SLIDER
	LabeledSlider* envDecaySlider = new LabeledSlider(processor.parameters, id_EnvDecay);
	envDecaySlider->setNumDecimalPlacesToDisplay(3);
	envDecaySlider->setSkewFactorFromMidPoint(0.01);
	envelopeGroupSliders.add(envDecaySlider);

	//SUSTAIN SLIDER
	LabeledSlider* envSustainSlider = new LabeledSlider(processor.parameters, id_EnvSustain);
	envSustainSlider->setNumDecimalPlacesToDisplay(3);
	envSustainSlider->setSkewFactorFromMidPoint(0.01);
	envelopeGroupSliders.add(envSustainSlider);

	//RELEASE SLIDER
	LabeledSlider* envReleaseSlider = new LabeledSlider(processor.parameters, id_EnvRelease);
	envReleaseSlider->setNumDecimalPlacesToDisplay(3);
	envReleaseSlider->setSkewFactorFromMidPoint(0.01);
	envelopeGroupSliders.add(envReleaseSlider);

	//GROUP COMPOSITION
	LabeledGroup* envelopeGroup = new LabeledGroup("Envelope", envelopeGroupSliders);
	mLabeledGroups.add(envelopeGroup);
	addAndMakeVisible(envelopeGroup);
	//filterGroup->setLookAndFeel(&labeledGroupLookAndFeel);
}

//DELAY
void AudioPlugInAudioProcessorEditor::createDelayGroup()
{
	Array<Component*> delayGroupSliders;

	//TIME SLIDER
	LabeledSlider* delayTimeSlider = new LabeledSlider(processor.parameters, id_DelTime);
	delayTimeSlider->setInterval(0.001);
	delayTimeSlider->setNumDecimalPlacesToDisplay(4);
	//delayTimeSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayTimeSlider);

	//FEEDBACK SLIDER
	LabeledSlider* delayFeedbackSlider = new LabeledSlider(processor.parameters, id_DelFeedback);
	delayFeedbackSlider->setInterval(0.01);
	delayFeedbackSlider->setNumDecimalPlacesToDisplay(2);
	//delayFeedbackSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayFeedbackSlider);

	//DRY/WET SLIDER
	LabeledSlider* delayDWMixSlider = new LabeledSlider(processor.parameters, id_DelDWMix);
	delayDWMixSlider->setInterval(0.01);
	delayDWMixSlider->setNumDecimalPlacesToDisplay(2);
	//delayDWMixSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayDWMixSlider);

	//PRE/POST SLIDER
	LabeledSlider* delayPrePostMixSlider = new LabeledSlider(processor.parameters, id_DelPrePostMix);
	delayPrePostMixSlider->setInterval(0.01);
	delayPrePostMixSlider->setNumDecimalPlacesToDisplay(2);
	//delayPrePostMixSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayPrePostMixSlider);

	//GROUP COMPOSITION
	LabeledGroup* delayGroup = new LabeledGroup("Delay", delayGroupSliders);
	mLabeledGroups.add(delayGroup);
	addAndMakeVisible(delayGroup);
	//filterGroup->setLookAndFeel(&labeledGroupLookAndFeel);
}
