/*
  ==============================================================================

    LabeledComponent.cpp
    Created: 25 Nov 2018 3:28:40pm
    Author:  Output Dev Laptop 2

  ==============================================================================
*/

#include "LabeledComponent.h"
#include "GlobalConstants.h"

LabeledComponent::LabeledComponent(LabeledComponentStyle inStyle,
                                   AudioProcessorValueTreeState& state,
                                   const String& parameterId)
:   mStyle(inStyle)
{
    switch(mStyle)
    {
        case(kLabeledComponentStyle_Button):{
            constructButton(state, parameterId);
        } break;
            
        case(kLabeledComponentStyle_Slider):{
            constructSlider(state, parameterId);
        } break;
            
        case(kLabeledComponentStyle_ComboBox):{
            constructComboBox(state, parameterId);
        } break;
    }
    
    setText(mName);
    setTextLabelPosition(Justification::centredTop);
    setSize(componentWidth, componentHeight);
    setLookAndFeel(&labeledComponentLookAndFeel); //Sets lookand feel for labeledslider, label, and group
}

LabeledComponent::~LabeledComponent()
{
	mSlider->setLookAndFeel(nullptr);
	setLookAndFeel(nullptr);
}

void LabeledComponent::resized()
{
    switch(mStyle)
    {
        case(kLabeledComponentStyle_Button):{
            buttonResized();
        } break;
            
        case(kLabeledComponentStyle_Slider):{
            sliderResized();
        } break;
            
        case(kLabeledComponentStyle_ComboBox):{
            comboBoxResized();
        } break;
    }
}

void LabeledComponent::sliderValueChanged (Slider* slider)
{
    updateLabelText();
}

void LabeledComponent::labelTextChanged (Label* labelThatHasChanged)
{
    const float value = labelThatHasChanged->getText().getFloatValue();
    
    if(mSlider){
        mSlider->setValue(value, sendNotification);
    } else {
        // slider doesn't exist!
        jassertfalse;
    }
}

void LabeledComponent::buttonClicked(Button* button)
{
	/*if (button->getState() = true) {
		mButton->setState false;
	}
	else {}
	}*/
    // do something..
}

void LabeledComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    // do something..
}

void LabeledComponent::setNumDecimalPlacesToDisplay(int decimals)
{
    mNumberOfDecimalsToDisplay = decimals;
}

void LabeledComponent::setInterval(float inNewInterval)
{
    Slider* slider = mSlider.get();
    
    if(slider){
        slider->setRange (mSliderRange.getRange().getStart(),
                          mSliderRange.getRange().getEnd(),
                          inNewInterval);
    } else {
        // what are you doing, this is not a slider!
        jassertfalse;
    }
}


void LabeledComponent::setSkewFactorFromMidPoint(double skewMidPoint)
{
	Slider* slider = mSlider.get();
	if (slider) {
		slider->setSkewFactorFromMidPoint(skewMidPoint);
	}
	else {
		// what are you doing, this is null!
		jassertfalse;
	}
}
void LabeledComponent::constructSlider(AudioProcessorValueTreeState& state,
                                       const String& parameterId)
{
    AudioProcessorParameterWithID* parameter =
    state.getParameter(parameterId);
    
    mName = parameter->name;
    
    const float defaultValue = parameter->getDefaultValue();
    mSliderRange = state.getParameterRange(parameterId);
    
    Slider* slider = new Slider();
    slider->setName(mName);
    slider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    slider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag); // OR RotaryHorizontalVerticalDrag
    slider->setValue(defaultValue, dontSendNotification);
    slider->setDoubleClickReturnValue(true, defaultValue);
    slider->setRange(mSliderRange.getRange().getStart(), mSliderRange.getRange().getEnd(), 0.001);
    slider->addListener(this);
    mSlider = std::unique_ptr<Slider>(slider);
    addAndMakeVisible(slider);
    
    mLabel = std::unique_ptr<Label>(new Label());
    mLabel->setEditable(true);
    mLabel->addListener(this);
    addAndMakeVisible(mLabel.get());
    updateLabelText();
    
    AudioProcessorValueTreeState::SliderAttachment* attachment =
    new AudioProcessorValueTreeState::SliderAttachment(state, parameterId, *slider);
    mSliderAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>(attachment);
}

void LabeledComponent::constructButton(AudioProcessorValueTreeState& state,
                                       const String& parameterId)
{
    AudioProcessorParameterWithID* parameter =
    state.getParameter(parameterId);
    
    mName = parameter->name;
    
    TextButton* button = new TextButton(); //TextButton(mName); for having button named.
    button->setClickingTogglesState(true);
    button->setToggleState(false, dontSendNotification);
    mButton = std::unique_ptr<TextButton>(button);
	addAndMakeVisible(button);
    
    AudioProcessorValueTreeState::ButtonAttachment* attachment =
    new AudioProcessorValueTreeState::ButtonAttachment(state, parameterId, *button);
	mButtonAttachment = std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>(attachment);
}

void LabeledComponent::constructComboBox(AudioProcessorValueTreeState& state,
                                         const String& parameterId)
{
    AudioProcessorParameterWithID* parameter =
    state.getParameter(parameterId);
    
    mName = parameter->name;
	
	ComboBox* combo = new ComboBox(); 
	combo->setEditableText(false);
	combo->setJustificationType(Justification::centred);
	mComboBox = std::unique_ptr<ComboBox>(combo);
	addAndMakeVisible(combo);

	AudioProcessorValueTreeState::ComboBoxAttachment* attachment =
		new AudioProcessorValueTreeState::ComboBoxAttachment(state, parameterId, *combo);
	mComboBoxAttachment = std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>(attachment);

}


void LabeledComponent::sliderResized()
{
    juce::Rectangle<int> sliderBounds = juce::Rectangle<int>(0, 0, getWidth(), getHeight()).reduced(componentMargin);
    //NEED TO CREATE ANOTHER RECTANGLE eg. knobBounds that is derived from a percent of sliderbounds
    
    juce::Rectangle<int> knobBounds = juce::Rectangle<int>(componentMargin,
                                                           componentMargin,
                                                           sliderBounds.getWidth(),
                                                           (sliderBounds.getHeight() - textBoxHeight)*0.9);
    
    
    juce::Rectangle<int> labelBounds = juce::Rectangle<int> ((sliderBounds.getWidth() * 0.5) - (textBoxWidth * 0.5) + componentMargin,
                                                             (sliderBounds.getHeight() - textBoxHeight)*0.95,
                                                             textBoxWidth,
                                                             textBoxHeight);
    
    Slider* slider = mSlider.get();
    slider->setBounds(knobBounds);
    
    Label* label = mLabel.get();
    label->setBounds(labelBounds);
}

void LabeledComponent::buttonResized()
{
	juce::Rectangle<int> generalBounds = juce::Rectangle<int>(0, 0, getWidth(), getHeight()).reduced(componentMargin);

	float buttonWidthPercent = 0.7f;
	float buttonHeight = 16.0f;
	float buttonWidth = 22.0;
	float buttonVerticalShift = 2.0f;

	juce::Rectangle<int> buttonBounds = juce::Rectangle<int>(
		//(generalBounds.getWidth() * 0.5) - (generalBounds.getWidth() * buttonWidthPercent * 0.5) + componentMargin,
		(generalBounds.getWidth() * 0.5) - (buttonWidth * 0.5) + componentMargin,
		(generalBounds.getHeight() * 0.5 - (buttonHeight * 0.5)) + componentMargin + buttonVerticalShift,
		//generalBounds.getWidth() * buttonWidthPercent,
		buttonWidth,
		buttonHeight);

	TextButton* button = mButton.get();
	button->setBounds(buttonBounds);
}

void LabeledComponent::comboBoxResized()
{
	juce::Rectangle<int> generalBounds = juce::Rectangle<int>(0, 0, getWidth(), getHeight()).reduced(componentMargin);

	float buttonWidthPercent = 0.7f;
	float buttonHeight = 14.0f;
	float buttonWidth = 35.0;
	float buttonVerticalShift = 2.0f;

	juce::Rectangle<int> comboBoxBounds = juce::Rectangle<int>(
		//(generalBounds.getWidth() * 0.5) - (generalBounds.getWidth() * buttonWidthPercent * 0.5) + componentMargin,
		(generalBounds.getWidth() * 0.5) - (buttonWidth * 0.5) + componentMargin,
		(generalBounds.getHeight() * 0.5 - (buttonHeight * 0.5)) + componentMargin + buttonVerticalShift,
		//generalBounds.getWidth() * buttonWidthPercent,
		buttonWidth,
		buttonHeight);

	ComboBox* combo = mComboBox.get();
	combo->setBounds(comboBoxBounds);
}

void LabeledComponent::updateLabelText()
{
    String value = String(mSlider->getValue(),
                          mNumberOfDecimalsToDisplay);
    
    mLabel->setText(value, dontSendNotification);
}
