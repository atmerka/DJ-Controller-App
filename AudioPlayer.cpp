/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 31 Oct 2021 1:51:38pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioPlayer.h"

//==============================================================================
AudioPlayer::AudioPlayer(AudioDeviceManager& deviceManager) : deviceManager(deviceManager)
{
    
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (0, 4, nullptr); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 4, nullptr);
    }
    
    
    
}

AudioPlayer::~AudioPlayer()
{
    
    shutdownAudio();
}


//==============================================================================
void AudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    
}

void AudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    bufferToFill.clearActiveBufferRegion();
    
    
    
    int buffSize = bufferToFill.buffer->getNumSamples();
    int songSamples = myWaveForm.getNumSamples();
    
    speedChangeBuffer.clear();
    //set legnth of my waveform to be the lenght of the incoming file
    speedChangeBuffer.setSize(2, buffSize);
    
    
    for(int c =0; c < bufferToFill.buffer->getNumChannels();++c)
    {
        auto* speedChangeData = speedChangeBuffer.getWritePointer(c);
//        auto* data = bufferToFill.buffer->getWritePointer(c);
    
        if(myWaveForm.getNumSamples()>0)
        {
            //when playTRACK  is called
            if(playing)
            {
                //if this is end of song
                if(outputSampleOffset+ buffSize*playSpeed> songSamples)
                {
                    //add  remaining track samples to buffer
                    bufferToFill.buffer->addFrom
                                    (c,
                                    0,
                                    myWaveForm,
                                    c,
                                    outputSampleOffset,
                                    songSamples-outputSampleOffset,
                                     gain); //gain
                    //pause the track
                    playing = false;
                }
               else if(outputSampleOffset+ buffSize*playSpeed<1)
               {
                   playing = false;
               }
                else
                {
                    
                    //add samples to our time change buffer with respect to the speed
                    for(int samp=0; samp<buffSize;samp++)
                    {
                        //check not at end
                        if(outputSampleOffset+int(samp*playSpeed)>songSamples) break;
                        
                        //add from song * playspeed
                        float val =myWaveForm.getSample(c, outputSampleOffset+int(samp*playSpeed));
                        speedChangeData[samp] =  val;
                        
                        //if its not null/zero avg b/t this and that
                    }
                    
                    bufferToFill.buffer->addFrom(c,
                                                 0,
                                                 speedChangeBuffer,
                                                 c,
                                                 0,
                                                 buffSize,
                                                 gain);
                }
            }
        }
        
    }
   
    
    //for fast forward/rewind
    if(ff)
    {
        playSpeed = 3.0f;
        if(outputSampleOffset+ buffSize*3>songSamples )
        {
            ff = false;
            playing = false;

            outputSampleOffset= songSamples-1;
        }
        
    }
    else if(rw)
    {
        playSpeed = -3.0f;
        if(outputSampleOffset- buffSize*3<1)
        {
            rw = false;
            playing = false;
            
            outputSampleOffset=1;
        }
    }
    if(playing)
    {
        //incremement playhead
        outputSampleOffset = outputSampleOffset + playSpeed* buffSize;
    }
  
}

void AudioPlayer::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}


void AudioPlayer::setAudioChannels (int numInputChannels, int numOutputChannels, const XmlElement* const xml)
{
    String audioError;
    
    if (usingCustomDeviceManager && xml == nullptr)
    {
        auto setup = deviceManager.getAudioDeviceSetup();
        
        if (setup.inputChannels.countNumberOfSetBits() != numInputChannels
            || setup.outputChannels.countNumberOfSetBits() != numOutputChannels)
        {
            setup.inputChannels.clear();
            setup.outputChannels.clear();
            
            setup.inputChannels.setRange (0, numInputChannels, true);
            setup.outputChannels.setRange (0, numOutputChannels, true);
            
            audioError = deviceManager.setAudioDeviceSetup (setup, false);
        }
    }
    else
    {
        audioError = deviceManager.initialise (numInputChannels, numOutputChannels, xml, true);
    }
    
    jassert (audioError.isEmpty());
    
    deviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (this);
}


void AudioPlayer::shutdownAudio()
{
    audioSourcePlayer.setSource (nullptr);
    deviceManager.removeAudioCallback (&audioSourcePlayer);
    
    // other audio callbacks may still be using the device
    if (! usingCustomDeviceManager)
        deviceManager.closeAudioDevice();
}
void AudioPlayer::paint (Graphics& g)
{
   
}
void AudioPlayer::loadWave(AudioBuffer<float> waveform)
{
    playing=false;
    outputSampleOffset =0;
    
    myWaveForm = waveform;
    
}
void AudioPlayer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void AudioPlayer::play(float speed)
{
    if(ff || rw) playSpeed = 1.0f;
    ff = false;
    rw = false;
    
    if(playSpeed == speed && playing == true){playing = false;}
    else{
    playSpeed = speed;
    playing = true;
    }
}
void AudioPlayer::fastForward()
{
    ff=true;
    playing = true;
    //adjust playspeed
    playSpeed = 3.0f;
}
void AudioPlayer::rewind()
{
    rw =true;
    playing = true;
    //adjust playspeed
    playSpeed = -3.0f;
}
void AudioPlayer::nudgeF(String type)
{
    if(playing && !nF && type == "start")
    {
        //adjust playspeed
        playSpeed = playSpeed*1.05f;
        nF = true;
    }
    else if(nF && type == "stop")
    {
        nF = false;
        playSpeed = playSpeed/1.05f;
    }
    
}
void AudioPlayer::nudgeB(String type)
{
    if(playing && !nB && type == "start")
    {
        //adjust playspeed
        playSpeed = playSpeed * 0.95f;
        nB = true;
    }
    else if(nB && type == "stop")
    {
        nB = false;
        playSpeed = playSpeed/0.95f;
    }
}
void AudioPlayer::newCueP(int cueP)
{
    //set new cuepoint
    cuePoint = cueP;
}
void AudioPlayer::playFromCue()
{
    //set sampleoffset to cuepoint
    setPos(cuePoint);
    
    //set playing
    playing = true;
    
    //playspeed
//    playSpeed = 1.0f;
}

void AudioPlayer::pause()
{
    playing = false;
}

void AudioPlayer::stop()
{
    playing = false;
    outputSampleOffset = 0;
}
void AudioPlayer::setGain(float g)
{
    gain = g;
}
void AudioPlayer::setSpeed(float s)
{
    playSpeed = s;
}

int AudioPlayer::getCuePoint()
{
    return cuePoint;
}

void AudioPlayer::setPos(int sampleOffset)
{
    outputSampleOffset = sampleOffset;
}

int AudioPlayer::getSampleOffset()
{
    return outputSampleOffset;
}
