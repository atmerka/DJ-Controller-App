/*
  ==============================================================================

    Controller.h
    Created: 31 Oct 2021 1:51:29pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioPlayer.h"
#include "SexyLook1.h"
//==============================================================================
/*
*/
class Controller    : public Component, public Slider::Listener,  public Button::Listener // public ChangeListener, 
{
public:
    Controller(AudioDeviceManager&);
    ~Controller();

    void paint (Graphics&) override;
    void resized() override;

    //    void changeListenerCallback (ChangeBroadcaster* source) override;
    //    void timerCallback() override;
    
    
    //FOR STATE/SETTINGS
    //states
    void setSettingsState();
    void setControllerState();
    String text;
    bool settings;
    
  
    
private:
    //FOR LOOK
    SexyLook1 newLook;
    
    
    //FOR CONTROLLING USING BUTTONS
    bool nudgeF = false;
    bool nudgeB = false;
    
    
    //define buttons and sliders
    TextButton rwB,ffB,nudgeLB,nudgeRB;
    
    TextButton setCueB,playB,playCueB;
    
    Slider tempoS, volS;
    
    Slider gainS, eqBS, eqMS, eqHS;
    
 
    //button and slider functions
    void sliderValueChanged(Slider *slider) override;
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;
    
    int cueCount =0;
    
    
    AudioDeviceManager& deviceManager;
    
    public:
    //audio player
    AudioPlayer ap;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Controller)
};
