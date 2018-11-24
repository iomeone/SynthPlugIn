/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GlobalConstants.h"
#include "LabeledSlider.h"
#include "GUIElements.h"

//==============================================================================
AudioPlugInAudioProcessorEditor::AudioPlugInAudioProcessorEditor (AudioPlugInAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	createGainGroup();
	createEnvelopeGroup();

	setResizable(true, true);
	setSize(screenWidth, screenHeight);
}

AudioPlugInAudioProcessorEditor::~AudioPlugInAudioProcessorEditor()
{
}

//==============================================================================
void AudioPlugInAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colour(65,65,65));


}

void AudioPlugInAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..


	FlexBox groupsBox;
	groupsBox.flexWrap = FlexBox::Wrap::wrap;
	groupsBox.justifyContent = FlexBox::JustifyContent::flexStart;
	groupsBox.alignContent = FlexBox::AlignContent::flexStart;


	for (int i = 0; i< mLabeledGroups.size(); i++) {

		int rowCount = (mLabeledGroups[i]->getWidth() / (getWidth() - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2) + (mLabeledGroups[i]->getWidth() % (getWidth() - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2) != 0));
		// rowCount = group specific multiplier to determine rows based on (Group's componentsArrayWidth)/(screenwidth - margins)), rounded up
		// rounding up via [ q = x / y + (x % y != 0) ]

		groupsBox.items.add(FlexItem(*mLabeledGroups[i])
			.withMinHeight((mLabeledGroups[i]->getHeight()) * rowCount
				+ verticalLabeledGroupMargin //comment out this line and it expands normally horizontally at least on resize. ***
				+ betweenLabeledComponentMargin * 2 * (rowCount - 1) //to compensate for inner margins for added rows
			)

			//.withMinWidth(getWidth() - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2)

			.withMinWidth(mLabeledGroups[i]->getWidth())
			//removing minWidth line allows better visualization of fundamental problem - labeledSliders are expanding horizontally to abnormal size on window contraction

			.withMaxWidth(getWidth() - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2)
			.withMargin(betweenLabeledGroupMargin) // adds margin between labeledGroups
			.withFlex(1));

	}

	FlexBox parentBox;
	parentBox.flexDirection = FlexBox::Direction::column;
	parentBox.items.add(FlexItem(groupsBox).withFlex(2.5, 2).withMargin(outerScreenMargin)); //adds margin around edge of screen
	parentBox.performLayout(getLocalBounds().toFloat());

}
