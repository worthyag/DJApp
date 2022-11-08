/*
  ==============================================================================

    DeckGUI.cpp
    Created: 21 Feb 2022 11:48:02am
    Author:  Worthy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager & formatManagerToUse,
                 AudioThumbnailCache &cacheToUse)
                : waveformDisplay(formatManagerToUse, cacheToUse),
                  player(_player)
{
    //==============================================================================
    // Changes the default LookAndFeel
    LookAndFeel::setDefaultLookAndFeel(&appLookAndFeel);

    //==============================================================================
    playPauseButton.setToggleState(true, NotificationType::dontSendNotification);
    playPauseButton.setImages(true, true, true, playOnBtn,
                            1.0, juce::Colours::transparentWhite, playOnBtn,
                            1.0, juce::Colour(), playOffBtn, 1.0, juce::Colour());
    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);

    //==============================================================================
    loadButton.setImages(true, true, true, loadBtn,
                       1.0, juce::Colours::transparentWhite, Image(), 1.0,
                       juce::Colour(), loadOverBtn, 1.0, juce::Colours::transparentWhite);
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    //==============================================================================
    rewindButton.setImages(true, true, true, rewindOnBtn,
                         1.0, juce::Colours::transparentWhite, rewindOffBtn, 1.0,
                         juce::Colour(), rewindOffBtn, 1.0, juce::Colours::transparentWhite);
    addAndMakeVisible(rewindButton);
    rewindButton.addListener(this);

    //==============================================================================
    forwardButton.setImages(true, true, true, forwardOnBtn,
                          1.0, juce::Colours::transparentWhite, forwardOffBtn, 1.0,
                          juce::Colour(), forwardOffBtn, 1.0, juce::Colours::transparentWhite);
    addAndMakeVisible(forwardButton);
    forwardButton.addListener(this);

    //==============================================================================
    loopButton.setToggleState(true, NotificationType::dontSendNotification);
    loopButton.setImages(true, true, true, loopOnBtn,
                       1.0, juce::Colours::transparentWhite, loopOffBtn, 1.0,
                       juce::Colour(), loopOffBtn, 1.0, juce::Colours::transparentWhite);
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);

    //==============================================================================
    volSlider.setColour(juce::Slider::backgroundColourId,
                      juce::Colour(0xffF8FBFF)); // pastel white-blue
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volSlider.setColour(juce::Slider::textBoxTextColourId,
                      juce::Colour(0xff1F1C1C)); // pastel black
    volSlider.setColour(juce::Slider::textBoxBackgroundColourId,
                      juce::Colour(0xffF8FBFF)); // pastel white-blue
    volSlider.setColour(juce::Slider::textBoxHighlightColourId,
                      juce::Colour(0xff585960)); //pastel dark grey
    volSlider.setColour(juce::Slider::textBoxOutlineColourId,
                      juce::Colour(0xff585960)); // pastel dark grey
    volSlider.setValue(0.2f);
    volSlider.updateText();

    addAndMakeVisible(volSlider);
    volSlider.addListener(this);

    volSlider.setRange(0.0f, 1.0f, 0.0001f); // increments the slider by 0.0001
    volSlider.setNumDecimalPlacesToDisplay(2);

    volLabel.setText("Volume", dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    addAndMakeVisible(volLabel);

    //==============================================================================
    speedSlider.setColour(juce::Slider::backgroundColourId,
                        juce::Colour(0xffF8FBFF)); // pastel white-blue
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    speedSlider.setColour(juce::Slider::textBoxTextColourId,
                        juce::Colour(0xff1F1C1C)); // pastel black
    speedSlider.setColour(juce::Slider::textBoxBackgroundColourId,
                        juce::Colour(0xffF8FBFF)); // pastel white-blue
    speedSlider.setColour(juce::Slider::textBoxHighlightColourId,
                        juce::Colour(0xff585960)); //pastel dark   grey
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId,
                        juce::Colour(0xff585960)); //pastel dark grey
    speedSlider.setValue(1.0);
    speedSlider.setSkewFactorFromMidPoint(5.0); // to make the changes more logarithmic

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);

    speedSlider.setRange(1.01f, 100.0f, 0.001f); // increments the slider by 0.001
    speedSlider.setNumDecimalPlacesToDisplay(2);

    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    addAndMakeVisible(speedLabel);

    //==============================================================================
    posSlider.setColour(juce::Slider::backgroundColourId,
                      juce::Colour(0xffF8FBFF)); // pastel white-blue
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    posSlider.setColour(juce::Slider::textBoxTextColourId,
                      juce::Colour(0xff1F1C1C)); // pastel black
    posSlider.setColour(juce::Slider::textBoxBackgroundColourId,
                      juce::Colour(0xffF8FBFF)); // pastel   white-blue
    posSlider.setColour(juce::Slider::textBoxHighlightColourId,
                      juce::Colour(0xff585960)); // pastel dark grey
    posSlider.setColour(juce::Slider::textBoxOutlineColourId,
                      juce::Colour(0xff585960)); // pastel dark grey
    posSlider.setValue(0.03);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    posSlider.addListener(this);

    posSlider.setRange(0.0f, 1.0f, 0.0001f); // increments the slider by 0.0001
    posSlider.setNumDecimalPlacesToDisplay(2);

    posLabel.setText("Position", dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);
    addAndMakeVisible(posLabel);

    //==============================================================================
    bVinyl.setToggleState(true, NotificationType::dontSendNotification);
    bVinyl.setImages(true, true, true, blueVinylLarge, 1.0,
                   juce::Colours::transparentWhite, blueVinylOver, 1.0,
                   juce::Colour(), blueVinyl, 1.0, juce::Colours::transparentWhite);
    addAndMakeVisible(bVinyl);

    //==============================================================================
    trackTitle.setText("Title", dontSendNotification);
    addAndMakeVisible(trackTitle);
    trackLength.setText("Duration", dontSendNotification);
    addAndMakeVisible(trackLength);

    //==============================================================================
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

//==============================================================================
void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background
    g.fillAll(juce::Colour(0xff62636D)); // pastel dark grey
    g.setColour(juce::Colour(0xff62636D)); // pastel dark grey
    g.drawRect(getLocalBounds(), 3.2); // draw an outline around the component
    g.setColour(juce::Colour(0xffFFFFFF));
    g.fillRect(getWidth() - 3, 0, 5, getHeight());
    g.fillRect(0, 0, 3, getHeight());
}

void DeckGUI::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.setGap(5_px);

    grid.templateRows = {
                           Track (Fr (1)), Track (Fr (1)),
                           Track (Fr (1)), Track (Fr (2)),
                           Track (Fr (2)), Track (Fr (1))
                        };

    grid.templateColumns = {
                               Track (Fr (1)), Track (Fr (2)),
                               Track (Fr (2)), Track (Fr (2)),
                               Track (Fr (2)), Track (Fr (2)),
                               Track (Fr (2)), Track (Fr (2)),
                               Track (Fr (2)), Track (Fr (1))
                            };

    grid.items = {
                      GridItem(&waveformDisplay).withArea(2, 2, 3, 7),
                      GridItem(&loadButton).withArea(6, 2, 6, 2),
                      GridItem(&loopButton).withArea(6, 6, 6, 6),
                      GridItem(&rewindButton).withArea(6, 3, 6, 3),
                      GridItem(&forwardButton).withArea(6, 5, 6, 5),
                      GridItem(&volSlider).withArea(3, 7, 7, 7),
                      GridItem(&speedSlider).withArea(3, 8, 7, 8),
                      GridItem(&posSlider).withArea(3, 9, 7, 9),
                      GridItem(&playPauseButton).withArea(6, 4, 6, 4),
                      GridItem(&bVinyl).withArea(3, 2, 6, 7),
                      GridItem(&trackTitle).withArea(1, 2, 1, 7),
                      GridItem(&trackLength).withArea(1, 7, 1, 10)
                 };

    grid.performLayout (getLocalBounds());
}

//==============================================================================
void DeckGUI::buttonClicked(Button* button)
{
    //==============================================================================
    /* the pointer is pointing to a space in memory. If that space of
     memory is the playpause button address then the following will occur. */
    if (button == &playPauseButton)
    {
        if (playState == PlayState::Stop) {
            // If the btn is clicked and its in a stopped state, + I click it, I want it to play
            playPauseButton.onClick = [this]() { play(); };
        }
        else if (playState == PlayState::Play)
            playPauseButton.onClick = [this]() { stop(); };
    }

    //==============================================================================
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen()) {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});
            trackTitle.setText(chooser.getResult().getFileNameWithoutExtension().toStdString(), dontSendNotification);
            trackLength.setText(getTrackLength(waveformDisplay.audioThumb.getTotalLength()), dontSendNotification);
        }
    }

    //==============================================================================
    if (button == &loopButton) {
    std::cout << "LoopButton was clicked...\n";

        if (loopState == LoopState::NoLoop) {
            // If the btn is clicked and its in a no looped state, + I click it, I want it to loop
            loopButton.onClick = [this]() { loop(); };
        }
        else if (loopState == LoopState::Loop) {
             // If the btn is clicked and its in a looped state- switches to nolooped state
              loopButton.onClick = [this]() { noLoop(); };
        }
    }

    //==============================================================================
    if (button == &rewindButton) {
        std::cout << "RewindButton was clicked...\n";
        player->setPositionRelative(player->getPositionRelative()- 0.03);
    }

    if (button == &forwardButton) {
        std::cout << "ForwardButton was clicked...\n";
        player->setPositionRelative(player->getPositionRelative()+ 0.03);
    }
}

//==============================================================================
void DeckGUI::play()
{
    // as when it is stopped its 'true', and when it is playing it's false.
    playState = PlayState::Play;

    playPauseButton.setToggleState(false, NotificationType::dontSendNotification);
    bVinyl.setToggleState(false, NotificationType::dontSendNotification);
    std::cout << "Play button was clicked " << std::endl;

    player->start();
}

void DeckGUI::stop()
{
    // as when it is playing its 'false', and when it is stopped it's true.
    playState = PlayState::Stop;

    playPauseButton.setToggleState(true, NotificationType::dontSendNotification);
    bVinyl.setToggleState(true, NotificationType::dontSendNotification);
    std::cout << "Stop button was clicked " << std::endl;

    player->stop();
}

//==============================================================================
void DeckGUI::loop()
{
    // as when it is not looped its 'true', and when it is looping it's false.
    loopState = LoopState::Loop;

    loopButton.setToggleState(false, NotificationType::dontSendNotification);
    std::cout << "Loop button was clicked " << std::endl;

    player->readerSource->setLooping(true);
}

void DeckGUI::noLoop()
{
    // as when it is looping its 'false', and when it is not looping it's true.
    loopState = LoopState::NoLoop;

    loopButton.setToggleState(true, NotificationType::dontSendNotification);
    std::cout << "NoLoop button was clicked " << std::endl;

    player->readerSource->setLooping(false);
}

//==============================================================================
void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
        player->setGain(slider->getValue());

    if (slider == &speedSlider)
        player->setSpeed(slider->getValue());

    if (slider == &posSlider)
        player->setPositionRelative(slider->getValue());
}

//==============================================================================
bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{juce::File{files[0]}});
        waveformDisplay.loadURL(URL{juce::File{files[0]}});
        trackTitle.setText(juce::File{files[0]}.getFileNameWithoutExtension().toStdString(),
                           dontSendNotification);
        trackLength.setText(getTrackLength(waveformDisplay.audioThumb.getTotalLength()),
                            dontSendNotification);
    }
}

//==============================================================================
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

//==============================================================================
bool DeckGUI::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails & /*dragSourceDetails*/)
{
    return true;
}

void DeckGUI::itemDropped (const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) {
  
    std::cout << "DeckGUI item has dropped from the playlist." << std::endl;
    DBG("The name is " << dragSourceDetails.description.toString());

    player->loadURL(URL{juce::File{dragSourceDetails.description.toString()}});
    waveformDisplay.loadURL(URL{juce::File{dragSourceDetails.description.toString()}});

    trackTitle.setText(setTrackTitle(dragSourceDetails), dontSendNotification);
    trackLength.setText(setTrackLength(dragSourceDetails), dontSendNotification);
}

bool DeckGUI::shouldDrawDragImageWhenOver()
{
    return true;
}

//==============================================================================
std::string DeckGUI::setTrackTitle(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
    juce::File track = juce::File{dragSourceDetails.description.toString()};
    std::string trackTitle = track.getFileNameWithoutExtension().toStdString();
    
    return trackTitle;
}

std::string DeckGUI::setTrackLength(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
    juce::URL track = URL{juce::File{dragSourceDetails.description.toString()}};
    double time = waveformDisplay.audioThumb.getTotalLength();
    std::string trackDuration = getTrackLength(time);
    
    return trackDuration;
}

std::string DeckGUI::getTrackLength(double time)
{
    /* To hold the corresponding int values and later
     format for time*/
    std::string minDuration, secDuration, hourDuration;
    
    /*
     Rounding adds a slight degree in accuracy (noticed 1 difference
     in 20 songs)
     */
    int seconds = (int)(round(time * 100) / 100);
    int secRemain = seconds % 60;
    int minutes = (seconds - secRemain) / 60;
    int minRemain = minutes % 60;
    int hours = (minutes - minRemain) / 60;
    
    // To format the time
    if (hours < 10) {
        hourDuration = "0" + std::to_string(hours);
    }
    else {
        hourDuration = std::to_string(hours);
    }
    
    if (minRemain < 10) {
        minDuration = "0" + std::to_string(minRemain);
    }
    else {
        minDuration = std::to_string(minRemain);
    }
    
    if (secRemain < 10) {
        secDuration = "0" + std::to_string(secRemain);
    }
    else {
        secDuration = std::to_string(secRemain);
    }
    
    // Create typical time styling
    std::stringstream calculateTime;
    calculateTime << hourDuration << ":" << minDuration << ":" << secDuration;
    
    std::string trackDuration = calculateTime.str();
    
    return trackDuration;
}
