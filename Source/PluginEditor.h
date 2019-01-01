/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LabeledGroup.h"
#include "LookAndFeel.h"
#include "MaxiOsc.h"

//==============================================================================
/**
*/
class AudioPlugInAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AudioPlugInAudioProcessorEditor (AudioPlugInAudioProcessor&);
    ~AudioPlugInAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
	AudioPlugInAudioProcessor& processor;
	
	void createGainGroup();
	void createEnvelopeGroup();
	void createNumPartialsGroup();
	void createDelayGroup();

	void createdummy1Group();
	void createdummy2Group();
	void createdummy3Group();
	void createdummy4Group();
	void createdummy5Group();


	FlexBox mFlexBox;
	FlexBox groupsBox;
	FlexBox parentBox;


	Viewport mainViewPort;
	Component generalBoundsComponent;


	OwnedArray<LabeledGroup> mLabeledGroups;
	LabeledGroupLookAndFeel labeledGroupLookAndFeel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlugInAudioProcessorEditor)
};
