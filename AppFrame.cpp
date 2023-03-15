/*
  ==============================================================================

    AppFrame.cpp
    Created: 31 Oct 2021 1:55:21pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AppFrame.h"

//==============================================================================
AppFrame::AppFrame() : contL(audioDeviceManager), contR(audioDeviceManager), flL(contL), flR(contR)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(800,600);
    
    
    //add child components
    addAndMakeVisible(contL);
    addAndMakeVisible(contR);
    addAndMakeVisible(flL);
    addAndMakeVisible(flR);
    addAndMakeVisible(settingsB);
    
    //set up settings button
    settingsB.addListener(this);
    settingsB.setToggleState(false, NotificationType::dontSendNotification);
    settingsB.setButtonText("settings");
    settingsB.setLookAndFeel(&newLook);
    
    startTimer(50);
    
    //AUDIO SETTINGS AND DEVICE MANAGER=================================================
#ifndef JUCE_DEMO_RUNNER
    RuntimePermissions::request (RuntimePermissions::recordAudio,
                                 [this] (bool granted)
                                 {
                                     int numInputChannels = granted ? 2 : 0;
                                     numInputChannels =0;
                                     audioDeviceManager.initialise (numInputChannels, 4, nullptr, true, {}, nullptr);
                                 });
#endif
    
    audioSetupComp.reset (new AudioDeviceSelectorComponent (audioDeviceManager,
                                                            0, 256, 0, 256, true, true, true, false));
    
    addAndMakeVisible (audioSetupComp.get());
    
    addAndMakeVisible (diagnosticsBox);
    diagnosticsBox.setMultiLine (true);
    diagnosticsBox.setReturnKeyStartsNewLine (true);
    diagnosticsBox.setReadOnly (true);
    diagnosticsBox.setScrollbarsShown (true);
    diagnosticsBox.setCaretVisible (false);
    diagnosticsBox.setPopupMenuEnabled (true);
    
    
    audioDeviceManager.addChangeListener (this);
    
    logMessage ("Audio device diagnostics:\n");
    dumpDeviceInfo();
    
}

AppFrame::~AppFrame()
{
    //device manager
    audioDeviceManager.removeChangeListener (this);
    //remove look and feel
    settingsB.setLookAndFeel(nullptr);
}

void AppFrame::paint (Graphics& g)
{

    //paint app frame
    g.fillAll (Colours::black);
    
    g.setColour(Colours::white);
    if(settings) g.drawText("SETTINGSON", 0, 0, getWidth(), getHeight()*0.1, Justification::centred);
    
}

void AppFrame::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    settingsB.setBounds(0, 0, getWidth(), getHeight()*0.1f);
    
    
    
    if(settings)
    {
        audioSetupComp->setBounds(0, getHeight()*0.1,getWidth(),getHeight()*0.6);
        
        
        diagnosticsBox.setBounds( 0, getHeight()*0.7,getWidth(),getHeight()*0.3);
        
    }
    else
    {
        //set up DJ MIXER
        flL.setBounds(0, getHeight()*0.1f, getWidth()/2, getHeight()*0.6f);
        contL.setBounds(0, getHeight()*0.7f, getWidth()/2, getHeight()*0.3f);
        contL.setControllerState();
        
        flR.setBounds(getWidth()/2, getHeight()*0.1f, getWidth()/2, getHeight()*0.6f);
        contR.setBounds(getWidth()/2, getHeight()*0.7f, getWidth()/2, getHeight()*0.3f);
        contR.setControllerState();
    }
    
    if(audioSetupComp.get() != nullptr)
    {
        audioSetupComp.get()->setVisible(settings);
        diagnosticsBox.setVisible(settings);
    }
    
    flL.setVisible(!settings);
    flR.setVisible(!settings);
    contR.setVisible(!settings);

}

void AppFrame::timerCallback()
{
    //if we load a file then load it in the audio player
    if(flL.fileLoaded())
    {
        //send playhead data to waveform image
        flL.updatePos(contL.ap.getSampleOffset());
        flL.updateCuePos(contL.ap.getCuePoint());
    }
    
    //if we load a file then load it in the audio player
    if(flR.fileLoaded())
    {
        //send playhead data to waveform image
        flR.updatePos(contR.ap.getSampleOffset());
        flR.updateCuePos(contR.ap.getCuePoint());
    }
    
    
}

void AppFrame::changeListenerCallback (ChangeBroadcaster* source)
{
    
    dumpDeviceInfo();
    
}

void AppFrame::dumpDeviceInfo()
{
    logMessage ("--------------------------------------");
    logMessage ("Current audio device type: " + (audioDeviceManager.getCurrentDeviceTypeObject() != nullptr
                                                 ? audioDeviceManager.getCurrentDeviceTypeObject()->getTypeName()
                                                 : "<none>"));
    
    if (AudioIODevice* device = audioDeviceManager.getCurrentAudioDevice())
    {
        logMessage ("Current audio device: "   + device->getName().quoted());
        logMessage ("Sample rate: "    + String (device->getCurrentSampleRate()) + " Hz");
        logMessage ("Block size: "     + String (device->getCurrentBufferSizeSamples()) + " samples");
        logMessage ("Output Latency: " + String (device->getOutputLatencyInSamples())   + " samples");
        logMessage ("Input Latency: "  + String (device->getInputLatencyInSamples())    + " samples");
        logMessage ("Bit depth: "      + String (device->getCurrentBitDepth()));
        logMessage ("Input channel names: "    + device->getInputChannelNames().joinIntoString (", "));
        logMessage ("Active input channels: "  + getListOfActiveBits (device->getActiveInputChannels()));
        logMessage ("Output channel names: "   + device->getOutputChannelNames().joinIntoString (", "));
        logMessage ("Active output channels: " + getListOfActiveBits (device->getActiveOutputChannels()));
    }
    else
    {
        logMessage ("No audio device open");
    }
    
}

void AppFrame::logMessage (const String& m)
{
    diagnosticsBox.moveCaretToEnd();
    diagnosticsBox.insertTextAtCaret (m + newLine);
}


void AppFrame::buttonClicked(Button* b)
{
    settings = b->getToggleState();
    resized();
}
