/*
  ==============================================================================

    FileLoader.h
    Created: 31 Oct 2021 1:55:33pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Visualiser.h"
#include "Controller.h"


//==============================================================================
/*
*/
class FileLoader    : public Component,public FileDragAndDropTarget , public Timer
{
public:
    FileLoader(Controller&);
    ~FileLoader();

    void paint (Graphics&) override;
    void resized() override;

    //for drawing song wave
    void updateWaveform(AudioBuffer<float>& song);
    
    //processor:
    int cuePoint=0;
    float playedPerct=0.0f;
    float tempo=0.0f;
    float trackTimeGone=0.0f;
    float trackTime=500.0f;
    String fName = "";
    bool playing = false;
    
    
    //GET WAVEFORM
    AudioBuffer<float> getWaveForm();
    bool fileLoaded();
    
    void setPlayedPercentage(float percent);
    void updatePos(int sampleOffset);
    int getPosition();
    void updateCuePos(int cueP);
    
    
private:
    
    void timerCallback() override;

    //FOR LOADING AND READING FILE
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    
    
    //format reader
    void loadFile(const String& path, String side);
    AudioFormatManager myFormatManager;
    AudioFormatReader* myFormatReader{nullptr};
    
    //this should somehow be a unique pointer but not working with rest
    AudioBuffer<float> myWaveForm;
    
    
    
    //for reading and playing song
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    
    
    //mouse position
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    void mouseDown (const MouseEvent& e) override;
    void setPos(int x);
    
    
    //FOR DRAWING WAVEFORM
    std::vector<float> waveValsToDrawL;
    std::vector<float> waveValsToDrawR;
    bool drawWave = false;
    float playPercent = 0.0f ;
    int songT;
    
    //FOR DRAWING MOVING WAVEFORM
    void updateMovingWaveform(AudioBuffer<float>& song);
    void updateMovingWaveformFFT(AudioBuffer<float>& song);
    std::vector<float> movingWaveToDrawL;
    std::vector<float> movingWaveToDrawR;
    std::vector<float> movingWaveToDrawFFT;
    
    
    
    
    //VISUALISER
    Visualiser vis;

    
    Controller& c;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileLoader)
};
