/*
  ==============================================================================

    SexyLook1.h
    Created: 5 Nov 2021 11:31:10pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>



class SexyLook1 : public LookAndFeel_V4
{
public:
    
    void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    void drawButtonText(Graphics&, TextButton&,
                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    void drawLinearSlider (Graphics&,
                           int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const Slider::SliderStyle,
                           Slider&) override;
    
    void drawToggleButton (Graphics&, ToggleButton&,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    
private:
    
};
    
