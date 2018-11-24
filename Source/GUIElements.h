/*
  ==============================================================================

    GUIElements.h
    Created: 24 Oct 2018 1:19:53am
    Author:  Mike

  ==============================================================================
*/

#include <iostream>

#pragma once


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
