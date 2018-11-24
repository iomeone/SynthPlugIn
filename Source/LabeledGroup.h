//
//  LabledGroup.h
//  flexbox - App
//
//  Created by Output Dev Laptop 2 on 10/21/18.
//

#ifndef LabledGroup_h
#define LabledGroup_h

#include "JuceHeader.h"
#include "LabeledSlider.h"

class LabeledGroup
:   public GroupComponent
{
public:

    LabeledGroup(const String& name, Array<Component*> inComponentArray);
	~LabeledGroup();

	void resized() override;

private:
	

	LabeledGroupLookAndFeel labeledGroupLookAndFeel;

    OwnedArray<Component> mComponentArray;
    OwnedArray<Slider> mSliderArray;
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledGroup)
};


#endif /* LabledGroup_hpp */
