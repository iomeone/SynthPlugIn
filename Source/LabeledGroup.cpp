//
//  LabledGroup.cpp
//  flexbox - App
//
//  Created by Output Dev Laptop 2 on 10/21/18.
//
#include "GlobalConstants.h"
#include "LabeledGroup.h"


LabeledGroup::LabeledGroup(const String& name, Array<Component*> inComponentArray)
{
	setText(name);
    setTextLabelPosition(Justification::centredTop);

	mComponentArray.addArray(inComponentArray);
	
	setLookAndFeel(&labeledGroupLookAndFeel);
	
    int component_x = 0;
    int component_w = 0;
    int component_h = 0;
    
    for(int i = 0; i < mComponentArray.size(); i++){
        
        Component* component = mComponentArray[i];
        addAndMakeVisible(component);
        
		
        component_w = component->getWidth() + betweenLabeledComponentMargin*2;
        component_h = component->getHeight();
		component_x = component_x + component_w;
        
    }
    
    setSize(component_x + (labeledGroupHorizontalMargin*2), component_h); 
	//adding double the horizontal margin needed to prevent wrapping and compensate for margin

}

LabeledGroup::~LabeledGroup()
{
	setLookAndFeel(nullptr);
}

void LabeledGroup::resized()
{
	    
    FlexBox knobBox;
    knobBox.flexWrap = FlexBox::Wrap::wrap;
    knobBox.justifyContent = FlexBox::JustifyContent::center;
    knobBox.alignContent = FlexBox::AlignContent::center;
	//knobBox.alignItems = FlexBox::AlignItems::flexStart;

    for(int i = 0; i < mComponentArray.size(); i++){
		knobBox.items.add(FlexItem(*mComponentArray[i])
			.withMinHeight(mComponentArray[i]->getHeight())
			.withMinWidth(mComponentArray[i]->getWidth())
			.withMargin(betweenLabeledComponentMargin) //applies space between labeledComponents
			.withFlex(1, 1));
			
    }
    
    FlexBox fb;
    //fb.flexDirection = FlexBox::Direction::row;
	//fb.alignItems = FlexBox::AlignItems::flexEnd;

	fb.items.add(FlexItem(knobBox)
		.withMargin(labeledGroupHorizontalMargin) //Adding a number here is interchangeable with adding a number to "reduced()" below and will create a left margin
		//.withWidth(getWidth()) // optional line
		.withFlex(2.5, 2.5));	
	
    fb.performLayout(getLocalBounds().translated(0,verticalLabeledComponentTranslation).reduced(0).expanded(0).toFloat());
}
