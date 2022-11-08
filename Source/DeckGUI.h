/*
  ==============================================================================

    DeckGUI.h
    Created: 21 Feb 2022 11:48:02am
    Author:  Worthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "DJLookAndFeel.h"

//==============================================================================
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::DragAndDropTarget,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer 
{
public:
    //==============================================================================
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager &   formatManagerToUse,
            AudioThumbnailCache &   cacheToUse);
    ~DeckGUI() override;

    //==============================================================================
    // Called when the button is clicked.
    void buttonClicked (Button *) override;
    // Called when the slider is clicked.
    void sliderValueChanged (Slider *slider) override;

    //==============================================================================
    // Callback to check whether this target is interested in the set of files being offered.
    bool isInterestedInFileDrag (const StringArray &files) override;
    // Callback to indicate that the user has dropped the files onto this component.
    void filesDropped (const StringArray &files, int x, int y) override;

    //==============================================================================
    // The user-defined callback routine that actually gets called periodically.
    void timerCallback() override;

    //==============================================================================
    // Starts the player and changes the state of the playPauseButton
    void play();
    // Stops the player and changes the state of the playPauseButton
    void stop();

    //==============================================================================
    // Allows the track to loop and changes the state of the loopButton
    void loop();
    // Stops the track from looping and changes the state of the loopButton
    void noLoop();

    //==============================================================================
    // Callback to check whether this target is interested in the type of object being dragged.
    virtual bool isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
    // Callback to indicate that the user has dropped something onto this component.
    virtual void itemDropped(const SourceDetails &dragSourceDetails) override;
    /* Overriding this allows the target to tell the drag container whether to draw
     the drag image while the cursor is over it. */
    virtual bool shouldDrawDragImageWhenOver() override;

    //==============================================================================
    /* Takes the drag source details and uses the description
     to returns the title of the track as a string. */
    std::string setTrackTitle(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails);
    /* Takes the drag source details and uses the description
     to returns the track duration as a string. */
    std::string setTrackLength(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails);
    /* Formats the duration into the HH::MM::SS format and returns the string
     Used by the setTrackLength() function. */
    std::string getTrackLength(double);

    //==============================================================================
    /* For the layout and custom of the app. */
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    //==============================================================================
    // Allows the button to alternate between play and pause.
    enum class PlayState
    {
      Play,
      Stop
    };

    PlayState playState { PlayState::Stop };
    ImageButton playPauseButton{ "play / pause" };
    Image playOnBtn = ImageCache::getFromMemory(BinaryData::PlayOnBtn_png,
                                              BinaryData::PlayOnBtn_pngSize);
    Image playOffBtn = ImageCache::getFromMemory(BinaryData::PlayOffBtn_png,
                                               BinaryData::PlayOffBtn_pngSize);

    //==============================================================================
    // Allows the button to alternate between loop and noloop.
    enum class LoopState
    {
      Loop,
      NoLoop
    };

    LoopState loopState { LoopState::NoLoop };
    ImageButton loopButton{ "loop" };
    Image loopOnBtn = ImageCache::getFromMemory(BinaryData::LoopOnBtn_png,
                                                BinaryData::LoopOnBtn_pngSize);
    Image loopOffBtn = ImageCache::getFromMemory(BinaryData::LoopOffBtn_png,
                                                 BinaryData::LoopOffBtn_pngSize);

    //==============================================================================
    // Allows the user to load a track into the deck.
    ImageButton loadButton{ "LOAD" };
    Image loadBtn = ImageCache::getFromMemory(BinaryData::LoadBtn_png,
                                            BinaryData::LoadBtn_pngSize);
    Image loadOverBtn = ImageCache::getFromMemory(BinaryData::LoadOverBtn_png,
                                                BinaryData::LoadOverBtn_pngSize);

    // Allows the user to rewind a track.
    ImageButton rewindButton { "rewind" };
    Image rewindOnBtn = ImageCache::getFromMemory(BinaryData::RewindOnBtn_png,
                                                BinaryData::RewindOnBtn_pngSize);
    Image rewindOffBtn = ImageCache::getFromMemory(BinaryData::RewindOffBtn_png,
                                                 BinaryData::RewindOffBtn_pngSize);

    // Allows the user to forward a track.
    ImageButton forwardButton { "forward" };
    Image forwardOnBtn = ImageCache::getFromMemory(BinaryData::ForwardOnBtn_png,
                                                 BinaryData::ForwardOnBtn_pngSize);
    Image forwardOffBtn = ImageCache::getFromMemory(BinaryData::ForwardOffBtn_png,
                                                  BinaryData::ForwardOffBtn_pngSize);

    //==============================================================================
    // The vinyl to add character to the app.
    ImageButton bVinyl{ "Turntable" };

    Image blueVinyl = ImageCache::getFromMemory(BinaryData::BlueVinyl_png,
                                                  BinaryData::BlueVinyl_pngSize);
    Image blueVinylLarge = ImageCache::getFromMemory(BinaryData::BlueVinylLarge_png,
                                                       BinaryData::BlueVinylLarge_pngSize);
    Image blueVinylOver = ImageCache::getFromMemory(BinaryData::BlueVinylOver_png,
                                                  BinaryData::BlueVinylOver_pngSize);

    //==============================================================================
    // The sliders and their corresponding labels.
    Slider volSlider;
    Label volLabel;
    Slider speedSlider;
    Label speedLabel;
    Slider posSlider;
    Label posLabel;

    //==============================================================================
    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    //==============================================================================
    // Customises the app.
    DJLookAndFeel appLookAndFeel;

    //==============================================================================
    // To display the title and duration in the deck.
    juce::Label trackTitle;
    juce::Label trackLength;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};


