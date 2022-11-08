/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 21 Feb 2022 12:09:34pm
    Author:  Worthy

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager &   formatManagerToUse,
                                 AudioThumbnailCache &   cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)
                          
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {}

//==============================================================================
void WaveformDisplay::paint (juce::Graphics& g)
  {
    g.setColour(juce::Colour(0xffFFFFFF)); // white
    g.drawRect (getLocalBounds(), 2);   // draw an outline around the component

    g.setColour (juce::Colour(0xffCCB9F5)); //pastel yellow

    if(fileLoaded)
    {
        audioThumb.drawChannel(g, //graphics
                                getLocalBounds(), //bounds
                                0, //start time
                                audioThumb.getTotalLength(), //endtime
                                0, //channel
                                0.8f //zoom
                            );
        
        // To stop the position of the playhead jumping to a very low number that crashes the program
        if((position > -100) && (position < 100))
        {
            g.setColour(juce::Colour(0xff1F1C1C));
            g.fillRect(position * getWidth(), 0, 3, getHeight());
        }
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized() {}

//==============================================================================
void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
    
    if (fileLoaded)
    {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    }
    else
        std::cout << "wfd: not loaded! " << std::endl;
}

//==============================================================================
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

//==============================================================================
void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position && pos > 0)
    {
        position = pos;
        repaint();
    }
}
