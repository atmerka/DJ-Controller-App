/*
  ==============================================================================

    Visualiser.h
    Created: 31 Oct 2021 1:55:42pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Visualiser    : public Component
{
public:
    Visualiser();
    ~Visualiser();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Visualiser)
};
