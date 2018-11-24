/*
  ==============================================================================

    LookAndFeel.h
    Created: 25 Oct 2018 9:39:10pm
    Author:  Mike

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


//ResizableWindow::backgroundColourId = Colour(0x000000);



class SliderLookAndFeel : public LookAndFeel_V4
{
public:
	SliderLookAndFeel()
	{
		//setColour(Slider::thumbColourId, Colours::red);
	}

	
//    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
//        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
//    {
//        auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(6); //Margin reduction for slider.
//
//        //auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
//        const float radius = jmin(bounds.getWidth() / 2, bounds.getHeight() / 2) - 2.0f;
//        const float centreX = x + width * 0.5f;
//        const float centreY = y + height * 0.5f + (radius *0.2); //controls vertical shift of center of knobs
//        const float rx = centreX - radius;
//        const float ry = centreY - radius;
//        const float rw = radius * 2.0f;
//        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
//        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
//
//        if (radius > 20.0f) //DESCRIBES BIGGER KNOB WITH PERCENTAGE ROTARY INDICATOR
//        {
//            if (slider.isEnabled())
//                g.setColour(slider.findColour(Slider::rotarySliderFillColourId).withAlpha(isMouseOver ? 1.0f : 0.7f));
//            else
//                g.setColour(Colour(0x80808080));
//
//            const float thickness = 0.7f;
//
//            {
//                Path filledArc;
//                filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
//                g.fillPath(filledArc);
//            }
//
//            {
//                const float innerRadius = radius * 0.2f;
//                Path p;
//                p.addTriangle(-innerRadius, 0.0f,
//                    0.0f, -radius * thickness * 1.1f,
//                    innerRadius, 0.0f);
//
//                p.addEllipse(-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
//
//                g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
//            }
//
//            if (slider.isEnabled())
//                g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
//            else
//                g.setColour(Colour(0x80808080));
//
//            Path outlineArc;
//            outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
//            outlineArc.closeSubPath();
//
//            g.strokePath(outlineArc, PathStrokeType(slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
//        }
//        else //DESCRIBES SMALLER KNOB WITH SIMPLER DESIGN
//        {
//            if (slider.isEnabled())
//                g.setColour(Colour(0xFF0099ff).withAlpha(isMouseOver ? 1.0f : 0.7f)); //0x14145495
//            else
//                g.setColour(Colour(0x80808080));
//
//            Path p;
//            p.addEllipse(-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
//            PathStrokeType(rw * 0.12f).createStrokedPath(p, p); //muliplier describes thickness of outer circle stroke
//
//            p.addRoundedRectangle(Rectangle<float>(radius * 0.15f, -radius * 0.3f, -radius * 0.3f, -radius * 0.6f), 0.0f, 0.0f);
//            //p.addRectangle(Rectangle<float>(radius * 0.15f, -radius * 0.3f, -radius * 0.3f, -radius * 0.6f));
//
//            //1st parameter x position must be half of the third parameter * -1 (width)
//            //2nd parameter y position dictates how far out the tick is
//            //3rd parameter dictates width of tick
//            //4th parameter dictates length of tick
//            //last two dictate corner 1 and corner 2 roundness
//
//            //p.addLineSegment(Line<float>(0.0f, -radius * 0.15f, 0.0f, -radius *0.8f), rw * 0.18f);
//            //2nd param describes how far from center to start
//            //4th param describes length
//            //last param describes thickness of line
//
//            g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
//        }
//    }


};

class LabeledGroupLookAndFeel : public LookAndFeel_V4
{
public:
	LabeledGroupLookAndFeel()
	{
	}

	void drawGroupComponentOutline(Graphics& g, int width, int height,
		const String& text, const Justification& position,
		GroupComponent& group)
	{
		const float textH = 15.0f; //controls header text
		const float indent = 3.0f; // controls contraction of lower aspect of border in all directions
		const float textEdgeGap = 4.0f; // controls gap to each side of label
		auto cs = 5.0f; //controls corner size

		Font f(textH);

		Path p;
		auto x = indent ;
		auto y = f.getAscent() - 3.0f; //controls y axis position of label text
		auto w = jmax(0.0f, width - x * 2.0f);
		auto h = jmax(0.0f, height - y - indent); // allows subtraction from vertical height of group eg. -3
		cs = jmin(cs, w * 0.5f, h * 0.5f);
		auto cs2 = 2.0f * cs;

		auto textW = text.isEmpty() ? 0 : jlimit(0.0f, jmax(0.0f, w - cs2 - textEdgeGap * 2), f.getStringWidth(text) + textEdgeGap * 2.0f);
		auto textX = cs + textEdgeGap;

		if (position.testFlags(Justification::horizontallyCentred))
			textX = cs + (w - cs2 - textW) * 0.5f;
		else if (position.testFlags(Justification::right))
			textX = w - cs - textW - textEdgeGap;

		p.startNewSubPath(x + textX + textW, y);
		p.lineTo(x + w - cs, y);

		p.addArc(x + w - cs2, y, cs2, cs2, 0, MathConstants<float>::halfPi);
		p.lineTo(x + w, y + h - cs);

		p.addArc(x + w - cs2, y + h - cs2, cs2, cs2, MathConstants<float>::halfPi, MathConstants<float>::pi);
		p.lineTo(x + cs, y + h);

		p.addArc(x, y + h - cs2, cs2, cs2, MathConstants<float>::pi, MathConstants<float>::pi * 1.5f);
		p.lineTo(x, y + cs);

		p.addArc(x, y, cs2, cs2, MathConstants<float>::pi * 1.5f, MathConstants<float>::twoPi);
		p.lineTo(x + textX, y);

		auto alpha = group.isEnabled() ? 1.0f : 0.5f;


		g.setColour(Colour(0xFF3366cc).withAlpha(0.5f));  //0x-ALPHA-RGB
		g.fillPath(p);

		g.setColour(group.findColour(GroupComponent::outlineColourId)
			.withMultipliedAlpha(alpha));

		g.strokePath(p, PathStrokeType(2.0f));

		g.setColour(group.findColour(GroupComponent::textColourId)
			.withMultipliedAlpha(alpha));
		g.setFont(f);
		g.drawText(text,
			roundToInt(x + textX), 0,
			roundToInt(textW),
			roundToInt(textH),
			Justification::centred, true);
	}

	Font getSliderPopupFont(Slider&)
	{
		return Font(10.0f, Font::bold);
	}


};


class LabeledComponentLookAndFeel : public LookAndFeel_V4
{
public:
	LabeledComponentLookAndFeel()
	{
	}
    
    /** label */
    void drawLabel (Graphics& g, Label& label) override
    {
        
        TextEditor* editor = label.getCurrentTextEditor();

		float labelCornerSize = 3;
        juce::Rectangle<float> roundedRect = juce::Rectangle<float> (labelCornerSize/2, labelCornerSize/2, label.getWidth()-labelCornerSize, label.getHeight()-labelCornerSize);

        g.setColour(Colour(0xFF0099ff).withAlpha(0.3f));
        g.fillRoundedRectangle(roundedRect, labelCornerSize);
        
        g.setColour(Colours::whitesmoke);
        g.drawRoundedRectangle(roundedRect, labelCornerSize, 1);
        
        
        
        if(!editor){
            g.setColour(Colours::whitesmoke);
            g.setFont(11.0f);
            g.drawFittedText(label.getText(), 0, 0, label.getWidth(), label.getHeight(), Justification::centred, 1);
        }
    }
    
    //virtual Font getLabelFont (Label&) = 0;
    //virtual BorderSize<int> getLabelBorderSize (Label&) = 0;
    
    
    /** slider */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(6); //Margin reduction for slider.
        
        //auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        const float radius = jmin(bounds.getWidth() / 2, bounds.getHeight() / 2) - 2.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f + (radius *0.2); //controls vertical shift of center of knobs
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
        
        if (radius > 20.0f) //DESCRIBES BIGGER KNOB WITH PERCENTAGE ROTARY INDICATOR
        {
            if (slider.isEnabled())
                g.setColour(slider.findColour(Slider::rotarySliderFillColourId).withAlpha(isMouseOver ? 1.0f : 0.7f));
            else
                g.setColour(Colour(0x80808080));
            
            const float thickness = 0.7f;
            
            {
                Path filledArc;
                filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
                g.fillPath(filledArc);
            }
            
            {
                const float innerRadius = radius * 0.2f;
                Path p;
                p.addTriangle(-innerRadius, 0.0f,
                              0.0f, -radius * thickness * 1.1f,
                              innerRadius, 0.0f);
                
                p.addEllipse(-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
                
                g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
            }
            
            if (slider.isEnabled())
                g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
            else
                g.setColour(Colour(0x80808080));
            
            Path outlineArc;
            outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
            outlineArc.closeSubPath();
            
            g.strokePath(outlineArc, PathStrokeType(slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
        }
        else //DESCRIBES SMALLER KNOB WITH SIMPLER DESIGN
        {
            if (slider.isEnabled())
                g.setColour(Colour(0xFF0099ff).withAlpha(isMouseOver ? 1.0f : 0.7f)); //0x14145495
            else
                g.setColour(Colour(0x80808080));
            
            Path p;
            p.addEllipse(-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
            PathStrokeType(rw * 0.12f).createStrokedPath(p, p); //muliplier describes thickness of outer circle stroke
            
            p.addRoundedRectangle(Rectangle<float>(radius * 0.15f, -radius * 0.3f, -radius * 0.3f, -radius * 0.6f), 0.0f, 0.0f);
            //p.addRectangle(Rectangle<float>(radius * 0.15f, -radius * 0.3f, -radius * 0.3f, -radius * 0.6f));
            
            //1st parameter x position must be half of the third parameter * -1 (width)
            //2nd parameter y position dictates how far out the tick is
            //3rd parameter dictates width of tick
            //4th parameter dictates length of tick
            //last two dictate corner 1 and corner 2 roundness
            
            //p.addLineSegment(Line<float>(0.0f, -radius * 0.15f, 0.0f, -radius *0.8f), rw * 0.18f);
            //2nd param describes how far from center to start
            //4th param describes length
            //last param describes thickness of line
            
            g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
        }
    }

    /** group component */
	void drawGroupComponentOutline(Graphics& g, int width, int height,
		const String& text, const Justification& position,
		GroupComponent& group) override
	{
		const float textH = 11; //controls header text
		const float indent = 3.0f;
		const float textEdgeGap = 4.0f;
		auto cs = 5.0f;

		Font f(textH);

		Path p;
		auto x = indent;
		auto y = f.getAscent() - 3.0f;
		auto w = jmax(0.0f, width - x * 2.0f);
		auto h = jmax(0.0f, height - y - indent);
		cs = jmin(cs, w * 0.5f, h * 0.5f);
		auto cs2 = 2.0f * cs;

		auto textW = text.isEmpty() ? 0 : jlimit(0.0f, jmax(0.0f, w - cs2 - textEdgeGap * 2), f.getStringWidth(text) + textEdgeGap * 2.0f);
		auto textX = cs + textEdgeGap;

		if (position.testFlags(Justification::horizontallyCentred))
			textX = cs + (w - cs2 - textW) * 0.5f;
		else if (position.testFlags(Justification::right))
			textX = w - cs - textW - textEdgeGap;

		p.startNewSubPath(x + textX + textW, y);
		p.lineTo(x + w - cs, y);

		p.addArc(x + w - cs2, y, cs2, cs2, 0, MathConstants<float>::halfPi);
		p.lineTo(x + w, y + h - cs);

		p.addArc(x + w - cs2, y + h - cs2, cs2, cs2, MathConstants<float>::halfPi, MathConstants<float>::pi);
		p.lineTo(x + cs, y + h);

		p.addArc(x, y + h - cs2, cs2, cs2, MathConstants<float>::pi, MathConstants<float>::pi * 1.5f);
		p.lineTo(x, y + cs);

		p.addArc(x, y, cs2, cs2, MathConstants<float>::pi * 1.5f, MathConstants<float>::twoPi);
		p.lineTo(x + textX, y);

		auto alpha = group.isEnabled() ? 1.0f : 0.5f;


		g.setColour(Colour(0xFF000000).withAlpha(0.5f));  //0x-ALPHA-RGB
		g.fillPath(p);

		g.setColour(group.findColour(GroupComponent::outlineColourId)
			.withMultipliedAlpha(alpha));

		g.strokePath(p, PathStrokeType(2.0f));

		g.setColour(group.findColour(GroupComponent::textColourId)
			.withMultipliedAlpha(alpha));
		g.setFont(f);
		g.drawText(text,
			roundToInt(x + textX), 0,
			roundToInt(textW),
			roundToInt(textH),
			Justification::centred, true);
	}

	Font getSliderPopupFont(Slider&)
	{
		return Font(10.0f, Font::bold);
	}


};
