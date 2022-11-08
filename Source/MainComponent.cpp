#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //==============================================================================
    /* Make sure you set the size of the component after
       you add any child components. */
      setSize(960, 540);

    //==============================================================================
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    //==============================================================================
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    g.fillAll(juce::Colour(0xff62636D)); //pastel dark grey
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
  
  juce::Grid grid;
  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;
  
  grid.templateRows    = { Track (Fr (3)), Track (Fr (2)) };
  grid.templateColumns = { Track (Fr (1)), Track (Fr (1))};
 
  grid.items = {
                 GridItem(&deckGUI1).withArea(1, 1, 1, 2),
                 GridItem(&deckGUI2).withArea(1, 2, 1, 3),
                 GridItem(&playlistComponent).withArea(2, 1, 2, 3)
               };

  grid.performLayout (getLocalBounds());

}
