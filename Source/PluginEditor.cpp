/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GlobalConstants.h"
#include "GUIElements.h"

//==============================================================================
AudioPlugInAudioProcessorEditor::AudioPlugInAudioProcessorEditor (AudioPlugInAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    double timer = Time::getMillisecondCounterHiRes();

    createNumPartialsGroup();
    createGainGroup();
    createEnvelopeGroup();
    createDelayGroup();
	createdummy1Group();
	createdummy2Group();
	createdummy3Group();
	createdummy4Group();
	createdummy5Group();

	setResizable(true, true);
	setSize(screenWidth, screenHeight);
    
    double currentTime = Time::getMillisecondCounterHiRes() - timer;
    DBG("construction time: " << currentTime << "ms.");
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


	for (int i = 0; i < mLabeledGroups.size(); i++) {

		int rowCount = (mLabeledGroups[i]->getWidth() / (getWidth() - scrollBarWidth - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2) + (mLabeledGroups[i]->getWidth() % (getWidth() - scrollBarWidth - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2) != 0));
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

			.withMaxWidth(getWidth() - scrollBarWidth - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2)
			.withMargin(betweenLabeledGroupMargin) // adds margin between labeledGroups
			.withFlex(1));

		generalBoundsComponent.addChildComponent(*mLabeledGroups[i]);
	}

	parentBox.flexDirection = FlexBox::Direction::column;
	parentBox.items.add(FlexItem(groupsBox).withFlex(2.5, 2).withMargin(outerScreenMargin)); //adds margin around edge of screen
	parentBox.performLayout(Rectangle<float>(outerScreenMargin + betweenLabeledGroupMargin, outerScreenMargin, (getWidth() - scrollBarWidth - outerScreenMargin * 2 - betweenLabeledGroupMargin * 2), getHeight()));
	//parentBox.performLayout(getLocalBounds().toFloat());

	addAndMakeVisible(generalBoundsComponent);
	generalBoundsComponent.setSize(getParentWidth(), (int)mLabeledGroups[mLabeledGroups.size() - 1]->getBottom() + outerScreenMargin + betweenLabeledGroupMargin);

	mainViewPort.setScrollBarsShown(true, false);
	mainViewPort.setScrollBarThickness(scrollBarWidth);
	mainViewPort.setSize(screenWidth, screenHeight - 3); //need at least minus 2 here to prevent glitching at bottom of screen, minus 2 = intermittent glitching
	mainViewPort.setViewedComponent(&generalBoundsComponent);
	addAndMakeVisible(mainViewPort);

}
