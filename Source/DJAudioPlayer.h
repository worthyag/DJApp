/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 21 Feb 2022 12:02:19pm
    Author:  Worthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class DJAudioPlayer : public AudioSource {
  public:
    //==============================================================================
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer() override;

    //==============================================================================
    // Tells the source to start playing.
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    // Called repeatedly to fetch subsequent blocks of audio data.
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    // Allows the source to release anything it no longer needs after playback has stopped.
    void releaseResources() override;

    //==============================================================================
    // Loads the given URL.
    void loadURL(URL audioURL);
    
    //==============================================================================
    // Sets a gain to apply to the audio data.
    void setGain(double gain);
    // Sets a speed to apply to the audio data.
    void setSpeed(double ratio);
    // Changes the current playback position in the source stream.
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    // get the relative position of the playhead
    double getPositionRelative();
    
    //==============================================================================
    // Starts playing (if a source has been selected).
    void start();
    // Stops playing.
    void stop();
    
    //==============================================================================
    // The reader to use as the data source.
    std::unique_ptr<AudioFormatReaderSource> readerSource;
  

private:
    //==============================================================================
    AudioFormatManager& formatManager;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DJAudioPlayer)
};
