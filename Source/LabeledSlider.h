//
//  LabeledSlider.h
//  flexbox - App
//
//  Created by Bruce Dawson on 10/21/18.
//

#include "GlobalConstants.h"
#ifndef LabeledSlider_h
#define LabeledSlider_h

#include "JuceHeader.h"

#include "LookAndFeel.h"

class LabeledSlider
:   public GroupComponent,
    public Slider::Listener,
    public Label::Listener
{
public:
	// parameter constructor
	LabeledSlider(AudioProcessorValueTreeState& state,
		const String& parameterId);

	~LabeledSlider();
    
    void resized() override;

	void setRange(double min, double max, double newInterval);
	void setNumDecimalPlacesToDisplay(int decimals);
	void setSkewFactorFromMidPoint(double skewMidPoint);
	void setValue(double newValue, NotificationType notification = sendNotification);
	void onValueChange();
	//void setLookAndFeel(LookAndFeel *newLookAndFeel);

	void setDoubleClickReturnValue(bool doubleClickEnable, double doubleClickValue);
	double getValue() const;
    
    void setInterval(float inNewInterval);
    
    
    // slider listener
    void sliderValueChanged (Slider* slider) override;

    void sliderDragStarted (Slider* slider) override;
    
    void sliderDragEnded (Slider* slider) override;
    
    // label listener
    void labelTextChanged (Label* labelThatHasChanged) override;
    
    void editorShown (Label* label, TextEditor& textEditor) override;

private:
    
    // internal
    void updateLabelText();
    
    int mNumberOfDecimalsToDisplay;
    
    NormalisableRange<float> mRange;
    
    std::unique_ptr<Label> mLabel;
	std::unique_ptr<Slider> mSlider;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;

	SliderLookAndFeel sliderLookAndFeel;
	LabeledComponentLookAndFeel labeledComponentLookAndFeel;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)
};


#endif /* LabeledSlider_hpp */
