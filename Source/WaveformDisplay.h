/*
  ==============================================================================

    WaveformDisplay.h
    Created: 21 Feb 2022 12:09:34pm
    Author:  Worthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

//==============================================================================
class WaveformDisplay    : public Component,
                           public ChangeListener
{
    friend class DeckGUI;
public:
    //==============================================================================
    WaveformDisplay( AudioFormatManager &   formatManagerToUse,
                    AudioThumbnailCache &   cacheToUse );
    ~WaveformDisplay();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    // Receives the callback.
    void changeListenerCallback(ChangeBroadcaster *source) override;

    //==============================================================================
    // Loads the given URL.
    void loadURL(URL audioURL);

    //==============================================================================
    // Set the relative position of the playhead.
    void setPositionRelative(double pos);

private:
    //==============================================================================
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
