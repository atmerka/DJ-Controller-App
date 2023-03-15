/*
  ==============================================================================

    AppFrame.h
    Created: 31 Oct 2021 1:55:21pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioPlayer.h"
#include "Controller.h"
#include "FileLoader.h"

//==============================================================================
/*
*/
class AppFrame    : public Component, public Timer, public Button::Listener,  public ChangeListener
{
public:
    AppFrame();
    ~AppFrame();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster* source) override;

    void timerCallback() override;
    
    bool hr=false;
private:
    
    //click button function
    void buttonClicked(Button*) override;
    
    //go to settings screen or main screen
    bool settings = false;
    
    //button to turn settings on and off
    ToggleButton settingsB;
    
    
    
    
    //DEVICE MANAGER
    #ifndef JUCE_DEMO_RUNNER
        AudioDeviceManager audioDeviceManager;
    #else
        AudioDeviceManager& audioDeviceManager { getSharedAudioDeviceManager() };
    #endif
        
    
    
    //FOR AUDIO SETTINGS
    std::unique_ptr<AudioDeviceSelectorComponent> audioSetupComp;
    TextEditor diagnosticsBox;
    
    
    void dumpDeviceInfo();
    void logMessage (const String& m);
    
    static String getListOfActiveBits (const BigInteger& b)
    {
        StringArray bits;
        
        for (int i = 0; i <= b.getHighestBit(); ++i)
            if (b[i])
                bits.add (String (i));
        
        return bits.joinIntoString (", ");
    }
    
    
    //PLAYS AUDIO
    Controller contL;
    Controller contR;
    
    //LOADS AUDIO FILES
    FileLoader flL;
    FileLoader flR;
    
    
    //FOR LOOK
    SexyLook1 newLook;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppFrame)
};
