/*
  ==============================================================================

    SexyLook1.cpp
    Created: 5 Nov 2021 11:31:10pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include "SexyLook1.h"


void SexyLook1::drawButtonBackground(Graphics &g, Button &b, const Colour& backgroundColour,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    
    g.setColour(Colours::darkblue);
    if(shouldDrawButtonAsHighlighted)
    {
        g.setColour(Colours::white);
    }
    
    if(shouldDrawButtonAsDown)
    {
        g.setColour(Colours::black);
    }
    g.fillAll() ;

    
   
}

void SexyLook1::drawButtonText(Graphics& g, TextButton& b,
               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    g.setColour(Colours::white);
    if(shouldDrawButtonAsHighlighted)
    {
        g.setColour(Colours::black);
    }
    
    if(shouldDrawButtonAsDown)
    {
        g.setColour(Colours::grey);
   }
    
    
    g.drawText(b.getButtonText(), 0, 0, b.getWidth(), b.getHeight(), Justification::centred);
}

void SexyLook1::drawLinearSlider (Graphics& g,
                      int x, int y, int width, int height,
                      float sliderPos,
                      float minSliderPos,
                      float maxSliderPos,
                      const Slider::SliderStyle style,
                      Slider& s)
{
    
    if(sliderPos <12 ) sliderPos = 12;
    
    if(style == Slider::LinearHorizontal)
    {
        if(sliderPos > width) sliderPos = width;
        
        //draw outline
        g.setColour(Colours::orange);
        g.drawRect(x, y, width, height);

        g.setColour(Colours::darkblue);
        g.fillRect(x+1, y+1,int(sliderPos-12) , height-2);
    }
    if(style == Slider::LinearVertical)
    {
        if(sliderPos>height) sliderPos = height;
        //draw outline
        g.setColour(Colours::orange);
        g.drawRect(x, y, width, height);
        
        g.setColour(Colours::darkblue);
        g.fillRect(x+1, y-1 +    int(sliderPos-12) , width-2 , height - int(sliderPos-12));
    }
}

void SexyLook1::drawToggleButton (Graphics& g, ToggleButton& b,
                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    
    g.fillAll(Colours::darkblue);
    
    
    if(b.getButtonText() == "settings")
    {
        g.setColour(Colours::white);
        g.drawText("CLICK 4 SETTINGS", 0, 0, b.getWidth(), b.getHeight(), Justification::centred);
    }
    
     if(b.getToggleState())
    {
        g.fillAll(Colours::darkred);
        g.setColour(Colours::white);
        {
              g.drawText("CLICK 4 DJ", 0, 0, b.getWidth(), b.getHeight(), Justification::centred);
        }
        
    }
    
}
