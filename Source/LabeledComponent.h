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
    
    NormalisableRange<float> mSliderRange;
    
    int mNumberOfDecimalsToDisplay;
    
    LabeledComponentStyle mStyle;
    
    std::unique_ptr<Label> mLabel;
    std::unique_ptr<Slider> mSlider;
    std::unique_ptr<Button> mButton;
    std::unique_ptr<ComboBox> mComboBox;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mButtonAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mComboBoxAttachment;
    
    LabeledComponentLookAndFeel labeledComponentLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledComponent)
};

