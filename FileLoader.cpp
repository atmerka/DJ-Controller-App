/*
  ==============================================================================

    FileLoader.cpp
    Created: 31 Oct 2021 1:55:33pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileLoader.h"


//==============================================================================
FileLoader::FileLoader(Controller& c) : c(c)
{
    
    startTimer(30);
    
    addAndMakeVisible(vis);
    
    Component::addMouseListener(this,true);

    //******************REMEMBER ME!!!**********************
    myFormatManager.registerBasicFormats();

    
}

FileLoader::~FileLoader()
{
    myFormatReader = nullptr;
    myFormatManager.clearFormats();
}

void FileLoader::paint (Graphics& g)
{
    //draw file loader
    g.fillAll(Colour(Colours::black));
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    
    //DRAWING MOVING WAVEFORM  ========================================
    
    if(drawWave)
    {
        //Have samples stored in an array
        updateMovingWaveform(myWaveForm);
        
        //draw a certain amount of the waveform
        //from 0.2-0.7 height, 0.1-0.9 width
        
        for(int i=0;i<getWidth()*0.8;i++)
        {
            //DRAW LEFT CHANNEL
            //colouring
            Colour c2 =   Colour::fromRGBA(100, 0, 100,200);
            Colour c3 =   Colour::fromRGBA(222, 0, 0,250);
            
            g.setColour(c2);
            if(movingWaveToDrawL[i]>0.8) g.setColour(c3);
            
            if(i>movingWaveToDrawL.size()) break;
            
            if(i>movingWaveToDrawR.size()) break;
            
            //scaling
            float yval = movingWaveToDrawL[i]*0.25*getHeight();
            //drawing
            g.drawLine(i+getWidth()*0.1,getHeight()*0.45+yval, i+getWidth()*0.1, getHeight()*0.45-yval);
            

            //DRAW RIGHT CHANNEL
            //colouring
            Colour c4 =   Colour::fromRGBA(0, 0, 200,50);
            Colour c5 =   Colour::fromRGBA(0, 222, 0,70);
            

            g.setColour(c4);
            
            if(movingWaveToDrawR[i]>0.8) g.setColour(c5);
            
            //scaling
            float yvalR = movingWaveToDrawR[i]*0.25*getHeight();
            //drawing
            g.drawLine(i+getWidth()*0.1,getHeight()*0.45+yvalR, i+getWidth()*0.1, getHeight()*0.45-yvalR);
            
            
        }
        
    }
    
    
    //DRAWING WAVEFORM =================================================
    //if we have a loaded waveform
    if(drawWave)
    {
        //set colour
        g.setColour(Colours::lightblue);
        
        
        //LEFT CHANNEL
        //scale for y-axis from -1 to 1 to the height of the screen
        for (int i = 0 ; i < waveValsToDrawL.size(); i++)
        {
            int b=0;
            //set colour
            if(waveValsToDrawL[i]>0.3) b = 100;//g.setColour(Colours::orchid);
            if(waveValsToDrawL[i]>0.4) b=150;//g.setColour(Colours::yellow);
            if(waveValsToDrawL[i]>0.7) b= 200;//g.setColour(Colours::greenyellow);
            if(waveValsToDrawL[i]>0.8) b = 256;//g.setColour(Colours::darkred);
            
            Colour c =   Colour::fromRGBA(50, 20, b,150);
            
            g.setColour(c);
            
            //scale the value at each index of myAudioPoints
            auto scaledYValue = jmap<float>(waveValsToDrawL[i], 0.0f, 1.0f,0,getHeight()*0.1);
            
            //draw bar representing the val of this sectino of samples
            g.drawLine(getWidth()*0.1 + i, getHeight()*0.8 + scaledYValue , getWidth()*0.1 + i,  getHeight()*0.8 -scaledYValue);
            
            
        }
        //RIGHT CHANNEL
        for (int i = 0 ; i < waveValsToDrawR.size(); i++)
        {
            int b=0;
            //set colour
            if(waveValsToDrawR[i]>0.3) b = 100;//g.setColour(Colours::orchid);
            if(waveValsToDrawR[i]>0.4) b=150;//g.setColour(Colours::yellow);
            if(waveValsToDrawR[i]>0.7) b= 200;//g.setColour(Colours::greenyellow);
            if(waveValsToDrawR[i]>0.8) b = 256;//g.setColour(Colours::darkred);
            
            Colour c =   Colour::fromRGBA(50, 20, b,150);
            
            g.setColour(c);
            
            //scale the value at each index of myAudioPoints
            auto scaledYValue = jmap<float>(waveValsToDrawR[i], 0.0f, 1.0f,0,getHeight()*0.1);
            
            //draw bar representing the val of this sectino of samples
            g.drawLine(getWidth()*0.1 + i, getHeight()*0.8 + scaledYValue , getWidth()*0.1 + i,  getHeight()*0.8 -scaledYValue);
            
            
        }
        
        
    }
    
    
        //drawing file name =======================================================
        g.setColour(Colours::orange);
        g.drawText(fName, 0, 0.95*getHeight(), getWidth(), 0.05*getHeight(), Justification::centred);
    
        //track time total
        g.drawText(String(trackTime) + " s", 0.2*getWidth(), 0.0*getHeight(),0.2*getWidth(),0.1*getHeight(), Justification::centred);
        //tracktime gone
        g.drawText(String(trackTimeGone) + " s", 0.0*getWidth(), 0.0*getHeight(),0.2*getWidth(),0.1*getHeight(), Justification::centred);
    
        //bpm
        g.drawText(String(tempo) + "bpm", 0.6*getWidth(), 0.0*getHeight(),0.4*getWidth(),0.1*getHeight(), Justification::centred);
    
    
        //drawing track length and play/cue heads ===== ===== ===== ===== ===== ===== =====
        //Track Length Line
        g.drawLine(getWidth()*.1, getHeight()*0.8, getWidth()*0.9, getHeight()*0.8);
    
        //playhead
        g.setColour(Colours::white);
        //    if(playPercent<=0.0f|| playPercent>1.0f) playPercent = 0.001f;
        //x position
        float x = getWidth()*(0.1) + playedPerct*0.8*getWidth();
        //checking errors
        if(x<=0 || x >= getWidth()) x = 0.5*getWidth();
        //draw
        g.drawLine(x, getHeight()*0.7, x, getHeight()*0.9);
    
    
        //cuehead
        g.setColour(Colours::greenyellow);
        //xpos
        float x1 = getWidth()*(0.1) + (cuePoint/(trackTime*44100))*0.8*getWidth();
        //errors
        if(x1<=0 || x1 >= getWidth()) x1 = 0.5*getWidth();
        //draw
        g.drawLine(x1, getHeight()*0.7, x1, getHeight()*0.9);
        //    g.drawText(String(xval) + "  " + String(yval), 0, 0.1*getHeight(), getWidth(), 10, Justification::centred);
    
    
    

}

void FileLoader::resized()
{
    
//    vis.setBounds(0, 0.1*getHeight(), getWidth(), getHeight()*0.8);
    if(drawWave) updateWaveform(myWaveForm);

}

bool FileLoader::isInterestedInFileDrag(const juce::StringArray &files)
{
    //for the files in the file array passed
    for(auto file: files)
    {
        //if it is of these formats
        if(file.contains(".wav" )|| file.contains (".mp3")||file.contains(".aif"))
        {
            //we can load it
            return true;
        }
    }
    return false;
}

void FileLoader::filesDropped(const StringArray& files, int x, int y)
{
    //for the files in the array
    for(auto file: files)
    {
        //if it is audio (from function above)
        if(isInterestedInFileDrag(file))
        {
            
            //load the file
            loadFile(file, "left");
            //updateWaveform();
            
            drawWave = false;
            updateWaveform(myWaveForm);
            //load the file from file loader to audioplayer
            c.ap.loadWave(myWaveForm);
        }
    }
    repaint();
}

void FileLoader::loadFile(const juce::String &path, String side)
{
    
    auto file = File (path);
    
    playing = false;
//    **stop playing


    fName = String(file.getFileName());

    
    
    //read it
    myFormatReader = myFormatManager.createReaderFor(file);
    
    
    //what is the lenghth of the in coming file?
    auto sampleLength = static_cast<int>(myFormatReader->lengthInSamples);
    
    myWaveForm.clear();
    //set legnth of my waveform to be the lenght of the incoming file
    myWaveForm.setSize(2, myFormatReader->lengthInSamples);
    
    trackTime = myWaveForm.getNumSamples()/44100;
    
    //i think this puts the sample into "mywaveform"
    //my wave form, start sample, cast as an integer the length in samples, start sample, left channel, right channel
    myFormatReader->read(&myWaveForm, 0, sampleLength, 0, true, true);
//    tempoL = tempoAnaylse(myWaveForm, myFormatReader->lengthInSamples);
    
    
    
    //anaylse tempo
    
    //reset cuepoint
    cuePoint =0;
    
    c.ap.newCueP(cuePoint);

    
    
}

void FileLoader::updateWaveform(AudioBuffer<float>& song)
{
    waveValsToDrawL.clear();
    
    waveValsToDrawR.clear();
    //how much smaller is our array than the song?
    int buffSize = song.getNumSamples()/int(getWidth()*0.8);
    
    //put them into our array
    for(int buffNo=0; buffNo<int(getWidth()*0.8);buffNo++)
    {
        //average value is equal to the rms of this buffer
        float avgVal = song.getRMSLevel(0, buffNo*buffSize, buffSize);
        waveValsToDrawL.push_back(avgVal);
        
        float avgValR = song.getRMSLevel(1, buffNo*buffSize, buffSize);
        waveValsToDrawR.push_back(avgValR);

    }
    //for drawing
    songT = getWidth()*0.8*buffSize/44100;
    //songT = song.getNumSamples() /44100;
    
    
    
    drawWave = true;
    
}
void FileLoader::mouseDown (const MouseEvent& e)
{
   
}


void FileLoader::mouseDrag (const MouseEvent& e)
{
    int xp = e.getMouseDownX();
    int yp = e.getMouseDownY();
    //set position via the mouse x value
    //give proportion of seek
    if(xp>getWidth() *0.1 && xp <getWidth()*0.9 && yp < getHeight()*0.9 && yp >getHeight()*0.7)
    {
        float xPropotion = xp/float(getWidth());
        xPropotion = (xPropotion-0.1)/0.8;
        
//        **** sets how far through the track to play
        c.ap.setPos(xPropotion*myWaveForm.getNumSamples());
        
        setPlayedPercentage(xPropotion);
//        playedPerct = xPropotion ;
        
        
       
    }
    
}


void FileLoader::setPlayedPercentage (float percent)
{
    playedPerct = percent;
    trackTimeGone = percent*trackTime;
    
    
}

void FileLoader::mouseUp (const MouseEvent&)
{
    
}


void FileLoader::timerCallback()
{
    repaint();
}

AudioBuffer<float> FileLoader::getWaveForm()
{
    
    return myWaveForm;
}

bool FileLoader::fileLoaded()
{
    return drawWave;
}
void FileLoader::updatePos(int sampleOffset)
{
    playedPerct = float(sampleOffset)/float(myWaveForm.getNumSamples());
    trackTimeGone = playedPerct * trackTime;
    
}
void FileLoader::updateCuePos(int cueP)
{
    cuePoint = cueP;
}

void FileLoader::updateMovingWaveform(AudioBuffer<float>& song)
{
    //for now we are clearing in future just move all along 1?
    //or use a cue - fifo
    movingWaveToDrawL.clear();
    movingWaveToDrawR.clear();
    
    int sampleOffset = c.ap.getSampleOffset();
    
    //put vals from song to our moving array
    for(int i=0;i<getWidth()*0.8;i++)
    {
        if(sampleOffset+i*100+100>song.getNumSamples()) return;
        //get the avg of 100 samples from the playhead(i+samplesoffset)
        //        float val = 0;
        //        float val = song.getRMSLevel(1, sampleOffset+ i*100, 100);
        float val = song.getMagnitude(0, sampleOffset+ i*200, 200);
        
        movingWaveToDrawL.push_back(val);
        
        float valR = song.getMagnitude(1, sampleOffset+ i*200, 200);
        
        movingWaveToDrawR.push_back(valR);
    }
}
