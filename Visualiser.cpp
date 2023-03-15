/*
  ==============================================================================

    Visualiser.cpp
    Created: 31 Oct 2021 1:55:42pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Visualiser.h"

//==============================================================================
Visualiser::Visualiser()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Visualiser::~Visualiser()
{
}

void Visualiser::paint (Graphics& g)
{
    
    //draw background
    g.fillAll(Colours::black);
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Visualiser", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Visualiser::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
