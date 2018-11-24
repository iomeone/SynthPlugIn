//
//  LabeledSlider.cpp
//  flexbox - App
//
//  Created by Output Dev Laptop 2 on 10/21/18.
//

#include "LabeledSlider.h"

LabeledSlider::LabeledSlider(AudioProcessorValueTreeState& state,
	const String& parameterId)
:   mNumberOfDecimalsToDisplay(0)
{
	AudioProcessorParameterWithID* parameter =
		state.getParameter(parameterId);

    const String name = parameter->name;
    const float defaultValue = parameter->getDefaultValue();
    mRange = state.getParameterRange(parameterId);

	Slider* slider = new Slider();
    slider->setName(name);
	slider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
	slider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider->setValue(defaultValue, dontSendNotification);
    slider->setDoubleClickReturnValue(true, defaultValue);
    slider->setRange(mRange.getRange().getStart(), mRange.getRange().getEnd(), 0.001);
    slider->addListener(this);
	mSlider = std::unique_ptr<Slider>(slider);
	addAndMakeVisible(slider);
    
    mLabel = std::unique_ptr<Label>(new Label());
    mLabel->setEditable(true);
    mLabel->addListener(this);
    addAndMakeVisible(mLabel.get());
    
    AudioProcessorValueTreeState::SliderAttachment* attachment =
    new AudioProcessorValueTreeState::SliderAttachment(state, parameterId, *slider);
    mAttachment = std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>(attachment);

	setText(name);
	setTextLabelPosition(Justification::centredTop);
	setSize(componentWidth, componentHeight);
	setLookAndFeel(&labeledComponentLookAndFeel); //Sets lookand feel for labeledslider, label, and group
}

LabeledSlider::~LabeledSlider()
{
	mSlider->setLookAndFeel(nullptr);
	setLookAndFeel(nullptr);
}

void LabeledSlider::resized()
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

void LabeledSlider::setRange(double min, double max, double newInterval = 0)
{
	Slider* slider = mSlider.get();
	slider->setRange(min, max, newInterval);
}

void LabeledSlider::setNumDecimalPlacesToDisplay(int decimals)
{
//    Slider* slider = mSlider.get();
//    slider->setNumDecimalPlacesToDisplay(decimals);
    
    mNumberOfDecimalsToDisplay = decimals;
}

void LabeledSlider::setSkewFactorFromMidPoint(double skewMidPoint)
{
	Slider* slider = mSlider.get();
	slider->setSkewFactorFromMidPoint(skewMidPoint);
}

void LabeledSlider::setValue(double newValue, NotificationType notification)
{
	Slider* slider = mSlider.get();
	slider->setValue(newValue, notification);
}

double LabeledSlider::getValue() const
{
	Slider* slider = mSlider.get();
	return slider->getValue();
}

void LabeledSlider::setInterval(float inNewInterval)
{
    Slider* slider = mSlider.get();
    slider->setRange (mRange.getRange().getStart(),
                      mRange.getRange().getEnd(),
                      inNewInterval);
}

void LabeledSlider::sliderValueChanged (Slider* slider)
{
    String value = String(mSlider->getValue(),
                          mNumberOfDecimalsToDisplay);

    mLabel->setText(value, dontSendNotification);
}

void LabeledSlider::sliderDragStarted (Slider* slider)
{
   // mLabel->setText((String)mSlider->getValue(), dontSendNotification);
}

void LabeledSlider::sliderDragEnded (Slider* slider)
{
   // mLabel->setText((String)mSlider->getName(), dontSendNotification);
}

void LabeledSlider::labelTextChanged (Label* labelThatHasChanged)
{
    const float value = labelThatHasChanged->getText().getFloatValue();
    
    mSlider->setValue(value, sendNotification);
}

void LabeledSlider::editorShown (Label* label, TextEditor& textEditor)
{
    juce::TextEditor::LengthAndCharacterRestriction* filter =
    new juce::TextEditor::LengthAndCharacterRestriction (10, "0123456789.");
    
    textEditor.setInputFilter(filter, true);
}

void LabeledSlider::onValueChange()
{
	Slider* slider = mSlider.get();
	slider->onValueChange();
}

void LabeledSlider::setDoubleClickReturnValue(bool doubleClickEnable, double doubleClickValue)
{
	Slider* slider = mSlider.get();
	slider->setDoubleClickReturnValue(doubleClickEnable, doubleClickValue);
}

