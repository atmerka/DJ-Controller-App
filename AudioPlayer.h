/*
  ==============================================================================

    AudioPlayer.h
    Created: 31 Oct 2021 1:51:38pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AudioPlayer    : public Component, public AudioSource
{
public:
    AudioPlayer(AudioDeviceManager&);
    ~AudioPlayer();

    void paint (Graphics&) override;
    void resized() override;
    
    //for audiosource
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //for audiodevice
    void setAudioChannels (int numInputChannels, int numOutputChannels, const XmlElement* const storedSettings = nullptr);
    void shutdownAudio();
    
    //load song
    void loadWave(AudioBuffer<float> waveform);
    
    
    //audio controll functions
    void play(float speed);
    void fastForward();
    void rewind();
    void nudgeF(String type);
    void nudgeB(String type);
    void newCueP(int cueP);
    void playFromCue();
    void pause();
    void stop();
    
      bool playing = false;
    
    //getters and setters
    int getSampleOffset();
    void setPos(int sampleOffset);
    int getCuePoint();
    
    void setGain(float g);
    void setSpeed(float s);
    
    
    
private:
    Random r;
    
    //song to play
    AudioBuffer<float> myWaveForm;
    
    
    //for tempo changing
    AudioBuffer<float> speedChangeBuffer;

    
    //playing variables
  
    int outputSampleOffset = 0;
    int cuePoint = 0;
    float playSpeed=1.0f;
    bool ff=false;
    bool rw=false;
    float gain;
    bool nF = false;
    bool nB = false;
    
    
    AudioDeviceManager& deviceManager;
    
    //    AudioDeviceManager defaultDeviceManager;
    
    
    AudioSourcePlayer audioSourcePlayer;
    bool usingCustomDeviceManager;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};
