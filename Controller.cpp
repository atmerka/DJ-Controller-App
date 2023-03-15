/*
  ==============================================================================

    Controller.cpp
    Created: 31 Oct 2021 1:51:29pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Controller.h"

//==============================================================================
Controller::Controller(AudioDeviceManager& deviceManager) : ap(deviceManager), deviceManager(deviceManager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    text = "Controller";
    
    
    //BUTTONS AND SLIDERS:
    //main buttons
    addAndMakeVisible(ffB);
    addAndMakeVisible(rwB);
    addAndMakeVisible(nudgeLB);
    addAndMakeVisible(nudgeRB);
    addAndMakeVisible(playCueB);
    addAndMakeVisible(setCueB);
    addAndMakeVisible(playB);
    
    ffB.setButtonText(">>");
    rwB.setButtonText("<<");
    nudgeLB.setButtonText("<|");
    nudgeRB.setButtonText("|>");
    playCueB.setButtonText("|<");
    setCueB.setButtonText("||");
    playB.setButtonText(">");
    
    
    playB.addListener(this);
    ffB.addListener(this);
    rwB.addListener(this);
    nudgeLB.addListener(this);
    nudgeRB.addListener(this);
    playCueB.addListener(this);
    setCueB.addListener(this);
    
    ffB.setLookAndFeel(&newLook);
    rwB.setLookAndFeel(&newLook);
    nudgeLB.setLookAndFeel(&newLook);
    nudgeRB.setLookAndFeel(&newLook);
    playCueB.setLookAndFeel(&newLook);
    setCueB.setLookAndFeel(&newLook);
    playB.setLookAndFeel(&newLook);
    
    //main sliders
    addAndMakeVisible(tempoS);
    addAndMakeVisible(volS);
    
    tempoS.setSliderStyle(Slider::LinearHorizontal);
    volS.setSliderStyle(Slider::LinearVertical);
    
    tempoS.setLookAndFeel(&newLook);
    volS.setLookAndFeel(&newLook);
    
    tempoS.setTextBoxStyle(Slider::NoTextBox, true, getWidth(), getHeight() );
    volS.setTextBoxStyle(Slider::NoTextBox, true, getWidth(), getHeight() );
    
    //eq knobs
    addAndMakeVisible(gainS);
    addAndMakeVisible(eqBS);
    addAndMakeVisible(eqMS);
    addAndMakeVisible(eqHS);
    
    gainS.setSliderStyle(Slider::RotaryVerticalDrag);
    eqHS.setSliderStyle(Slider::RotaryVerticalDrag);
    eqMS.setSliderStyle(Slider::RotaryVerticalDrag);
    eqBS.setSliderStyle(Slider::RotaryVerticalDrag);
    
    gainS.setTextBoxStyle(Slider::NoTextBox, true, getWidth(), getHeight() );
    eqHS.setTextBoxStyle(Slider::NoTextBox, true, getWidth(), getHeight() );
    eqMS.setTextBoxStyle(Slider::NoTextBox, true, getWidth(), getHeight() );
    eqBS.setTextBoxStyle(Slider::NoTextBox, true, getWidth(), getHeight() );
    
    
    volS.setValue(0.9f);
    tempoS.setValue(1.0f);
    
    volS.setRange(0.0f, 1.0f);
    tempoS.setRange(0.5f, 2.0f);
    tempoS.setSkewFactorFromMidPoint(1.0f);
    
    volS.addListener(this);
    tempoS.addListener(this);
    
    
}

Controller::~Controller()
{
//    //device manager
//    audioDeviceManager.removeChangeListener (this);
    
    ffB.setLookAndFeel(nullptr);
    rwB.setLookAndFeel(nullptr);
    nudgeLB.setLookAndFeel(nullptr);
    nudgeRB.setLookAndFeel(nullptr);
    playCueB.setLookAndFeel(nullptr);
    setCueB.setLookAndFeel(nullptr);
    playB.setLookAndFeel(nullptr);
    
    
}




void Controller::paint (Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

   
}

void Controller::resized()
{
    
    //IF WE ARE IN THE SETTINGS STATE
    if(settings)
    {
        
        rwB.setVisible(false);
        ffB.setVisible(false);
        nudgeLB.setVisible(false);
        nudgeRB.setVisible(false);
        setCueB.setVisible(false);
        playCueB.setVisible(false);
        playB.setVisible(false);
        tempoS.setVisible(false);
        volS.setVisible(false);
    }
    //ELSE IS NORMAL
    else
    {

        rwB.setVisible(true);
        ffB.setVisible(true);
        nudgeLB.setVisible(true);
        nudgeRB.setVisible(true);
        setCueB.setVisible(true);
        playCueB.setVisible(true);
        playB.setVisible(true);
        tempoS.setVisible(true);
        volS.setVisible(true);
        
        //WHERE WE SIZE OUR CONTROLLER
        int h = getHeight();
        int w = getWidth();
        
        //ff, rw, nudges
        rwB.setBounds(w*.05, h*.3, w*0.3, h*0.2);
        ffB.setBounds(w*.55, h*.3, w*0.3, h*0.2);
        
        nudgeLB.setBounds(w*.05, h*.525, w*0.3, h*0.2);
        nudgeRB.setBounds(w*.55, h*.525, w*0.3, h*0.2);
        
        //cue n play
        setCueB.setBounds(w*.05, h*.75, w*0.3, h*0.2);
        playCueB.setBounds(w*.35, h*.75, w*0.2, h*0.2);
        playB.setBounds(w*.55, h*.75, w*0.3, h*0.2);
        
        //tempo/vol
        tempoS.setBounds(w*.05, h*.1, w*.8, h*0.1);
        volS.setBounds(w*.9, h*.1, w*0.05, h*.8);
        
    }
    

}


void Controller::setSettingsState()
{
    text = ""; //"SETTINGS";
    settings = true;
    resized();
    
}


void Controller::setControllerState()
{
    text = "Controller";
    settings = false;
    resized();
}

void Controller::sliderValueChanged(Slider *slider)
{
    if (slider == &volS)
    {
        //processor.gainL = volS.getValue();
        //** PROCESSOR GAIN CHANGE
        ap.setGain(volS.getValue());
    }
    if(slider == &tempoS)
    {
//        processor.playspeedL = tempoS.getValue();
        //PROCESSOR TEMPO CHANGE ***
        ap.setSpeed(tempoS.getValue());
    }
}

void Controller::buttonStateChanged(Button* button)
{
    if(button->isDown())
    {
        
        //start analysing
        if(button->getButtonText() == ">")
        {
         
            ap.play(tempoS.getValue());
            
        }
        
        if(button->getButtonText() == ">>")
        {
//            processor.ffL = true;
            ap.fastForward();
            
        }
        if(button->getButtonText() == "<<")
        {
            ap.rewind();
//            processor.rwL = true;
           
        }
        
        if(button->getButtonText() == "||")
        {
            ap.newCueP(ap.getSampleOffset());
            
        }
        if(button->getButtonText() == "|<")
        {
            ap.playFromCue();
            
        }
        
        
    }
    
    if(button->getButtonText() == "|>")
    {
        if(button->isDown())
        {
            ap.nudgeF("start");
            
        }
        else if(!button->isDown())
        {
          ap.nudgeF("stop");
          
        }
    }
    
    if(button->getButtonText() == "<|")
    {
        //if button down
        if(button->isDown())
        {
            //start nudging
            ap.nudgeB("start");
        }
        else
        {
            //stop nudging
            ap.nudgeB("stop");
        }
    }
}

void Controller::buttonClicked(Button* button)
{
    
}

