/*
  ==============================================================================

    LabeledComponent.h
    Created: 25 Nov 2018 3:28:40pm
    Author:  Bruce Dawson

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "LookAndFeel.h"

/** DOXYGEN style

    This is a labeled component class that can take the form of a button, comboBox, or slider.
    These are found in the PluginEditor.
 
    @see    AudioPlugInAudioProcessorEditor
*/

enum LabeledComponentStyle
{
    kLabeledComponentStyle_Slider = 0,
    kLabeledComponentStyle_Button,
    kLabeledComponentStyle_ComboBox,
};

class LabeledComponent
:   public GroupComponent,
    public Slider::Listener,
    public Label::Listener,
    public Button::Listener,
    public ComboBox::Listener
{
public:
    
    LabeledComponent(LabeledComponentStyle inStyle,
                     AudioProcessorValueTreeState& state,
                     const String& parameterId);
    
	~LabeledComponent();

    void resized() override;
    
    /** slider listener */
    void sliderValueChanged (Slider* slider) override;
    
    /** label listener */
    void labelTextChanged (Label* labelThatHasChanged) override;
    
    /** button listener */
    void buttonClicked(Button* button) override;
    
    /** comboBox listener */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void setNumDecimalPlacesToDisplay(int decimals);
    void setInterval(float inNewInterval);
	void setSkewFactorFromMidPoint(double skewMidPoint);
    
    /** for doing component specific things like adding items to a comboBox.
    
     example:
     ComboBox* cb = mLabeledComboBox->getComboBox();
     cb->addItem("my item", 1);
     cb->addItem("my item", 2);
     cb->addItem("my item", 3);
     
    */
    
    Slider* getSlider() { return mSlider.get(); }
    
    Button* getButton() { return mButton.get(); }
    
    ComboBox* getComboBox() { return mComboBox.get(); }
    
    
    /** this function returns the associated range with our parameter component.
     
        for a slider, range might be 0.01 - 1.0f
        for a comboBox, range might be (0 - 4)
        for a button, range is (0 - 1)
    */
    Range<float> getRange() { return mRange.getRange(); }
    
private:
    
    /** internal */
    void constructSlider(AudioProcessorValueTreeState& state,
                         const String& parameterId);
    /** internal */
    void constructButton(AudioProcessorValueTreeState& state,
                         const String& parameterId);
    /** internal */
    void constructComboBox(AudioProcessorValueTreeState& state,
                           const String& parameterId);
    
    /** internal */
    void sliderResized();
    /** internal */
    void buttonResized();
    /** internal */
    void comboBoxResized();
    /** internal */
    void updateLabelText();
    
    String mName;
    
    NormalisableRange<float> mRange;
    
    int mNumberOfDecimalsToDisplay;
    
    LabeledComponentStyle mStyle;
    
    std::unique_ptr<Label> mLabel;
    std::unique_ptr<Slider> mSlider;
    std::unique_ptr<TextButton> mButton;
    std::unique_ptr<ComboBox> mComboBox;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mButtonAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mComboBoxAttachment;
    
    LabeledComponentLookAndFeel labeledComponentLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledComponent)
};

