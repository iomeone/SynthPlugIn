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
	LabeledComponent* numVoicesSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_NumVoices);
	numVoicesSlider->setInterval(1.0f);
	numVoicesSlider->setNumDecimalPlacesToDisplay(0);
	//numVoicesSlider->setSkewFactorFromMidPoint(40.0f);
	numPartialsGroupSliders.add(numVoicesSlider);

	LabeledComponent* numPartialsSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_NumPartials);
	numPartialsSlider->setInterval(1.0f);
	numPartialsSlider->setNumDecimalPlacesToDisplay(0);
	numPartialsSlider->setSkewFactorFromMidPoint(40.0f);
	numPartialsGroupSliders.add(numPartialsSlider);

	LabeledComponent* numUnisonSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_NumUnison);
	numUnisonSlider->setInterval(1.0f);
	numUnisonSlider->setNumDecimalPlacesToDisplay(0);
	//numPartialsSlider->setSkewFactorFromMidPoint(40.0f);
	numPartialsGroupSliders.add(numUnisonSlider);

	LabeledComponent* deTuneUnisonHzSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_DeTuneUnisonHz);
	deTuneUnisonHzSlider->setInterval(0.1f);
	deTuneUnisonHzSlider->setNumDecimalPlacesToDisplay(2);
	//deTuneUnisonHzSlider->setSkewFactorFromMidPoint(40.0f);
	numPartialsGroupSliders.add(deTuneUnisonHzSlider);

	LabeledComponent* samplesPerIncrementSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_SamplesPerIncrement);
	samplesPerIncrementSlider->setInterval(1);
	samplesPerIncrementSlider->setNumDecimalPlacesToDisplay(0);
	//samplesPerIncrementSlider->setSkewFactorFromMidPoint(40);
	numPartialsGroupSliders.add(samplesPerIncrementSlider);
	
	LabeledGroup* numPartialsGroup = new LabeledGroup("Setup", numPartialsGroupSliders);
	mLabeledGroups.add(numPartialsGroup);
	addAndMakeVisible(numPartialsGroup);
	//gainGroup->setLookAndFeel(&labeledGroupLookAndFeel);
}

//GAIN
void AudioPlugInAudioProcessorEditor::createGainGroup()
{
	Array<Component*> gainGroupSliders;

	//GAIN SLIDER
	LabeledComponent* gainLevelSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_SynthGain);
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
    
	//PRE-ATTACK SLIDER
	LabeledComponent* envPreAttackSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_EnvPreAttack);
	envPreAttackSlider->setInterval(0.001);
	envPreAttackSlider->setNumDecimalPlacesToDisplay(3);
	//envPreAttackSlider->setSkewFactorFromMidPoint(0.01); 
	envelopeGroupSliders.add(envPreAttackSlider);

	//PRE-ATTACK DECAY SLIDER
	LabeledComponent* envPreAttackDecaySlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_EnvPreAttackDecay);
	envPreAttackDecaySlider->setInterval(0.001);
	envPreAttackDecaySlider->setNumDecimalPlacesToDisplay(3);
	//envPreAttackDecaySlider->setSkewFactorFromMidPoint(0.01); 
	envelopeGroupSliders.add(envPreAttackDecaySlider);

	//ATTACK SLIDER
	LabeledComponent* envAttackSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_EnvAttack);
	envAttackSlider->setInterval(0.001);
	envAttackSlider->setNumDecimalPlacesToDisplay(3);
	//envAttackSlider->setSkewFactorFromMidPoint(0.01); 
	envelopeGroupSliders.add(envAttackSlider);

	//DECAY SLIDER
	LabeledComponent* envDecaySlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_EnvDecay);
	envDecaySlider->setInterval(0.001);
	envDecaySlider->setNumDecimalPlacesToDisplay(3);
	envDecaySlider->setSkewFactorFromMidPoint(0.01);
	envelopeGroupSliders.add(envDecaySlider);

	//SUSTAIN SLIDER
	LabeledComponent* envSustainSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_EnvSustain);
	envSustainSlider->setInterval(0.001);
	envSustainSlider->setNumDecimalPlacesToDisplay(3);
	envSustainSlider->setSkewFactorFromMidPoint(0.4);
	envelopeGroupSliders.add(envSustainSlider);

	//RELEASE SLIDER
	LabeledComponent* envReleaseSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_EnvRelease);
	envReleaseSlider->setInterval(0.001);
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

	//ON/OFF BUTTON
	LabeledComponent* delayOnOffButton = new LabeledComponent(kLabeledComponentStyle_Button, processor.parameters, id_DelOnOff);
	//delayOnOffButton->setInterval(0.001);
	delayGroupSliders.add(delayOnOffButton);

	//TIME SLIDER
	LabeledComponent* delayTimeSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_DelTime);
	delayTimeSlider->setInterval(0.001);
	delayTimeSlider->setNumDecimalPlacesToDisplay(2);
	//delayTimeSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayTimeSlider);

	//FEEDBACK SLIDER
	LabeledComponent* delayFeedbackSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_DelFeedback);
	delayFeedbackSlider->setInterval(0.01);
	delayFeedbackSlider->setNumDecimalPlacesToDisplay(2);
	//delayFeedbackSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayFeedbackSlider);

	//DRY/WET SLIDER
	LabeledComponent* delayDWMixSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_DelDWMix);
	delayDWMixSlider->setInterval(0.01);
	delayDWMixSlider->setNumDecimalPlacesToDisplay(2);
	//delayDWMixSlider->setSkewFactorFromMidPoint(0.5);
	delayGroupSliders.add(delayDWMixSlider);

	//PRE/POST SLIDER
	LabeledComponent* delayPrePostMixSlider = new LabeledComponent(kLabeledComponentStyle_Slider, processor.parameters, id_DelPrePostMix);
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
