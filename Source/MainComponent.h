#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::DragAndDropContainer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    // Tells the source to start playing.
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    // Called repeatedly to fetch subsequent blocks of audio data.
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    // Allows the source to release anything it no longer needs after playback has stopped.
    void releaseResources() override;

    //==============================================================================
    // Determines how the objects are rendered on the screen.
    void paint (juce::Graphics& g) override;
    // Called when this component's size has been changed.
    void resized() override;

private:
    //==============================================================================
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache} ;

    PlaylistComponent playlistComponent{ formatManager };

    MixerAudioSource mixerSource;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
