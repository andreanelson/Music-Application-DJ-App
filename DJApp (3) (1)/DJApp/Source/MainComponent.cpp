
#include "MainComponent.h"
//==============================================================================

MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

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
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //preparing audio for player 1 and 2
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    //preparing audio for mixer
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

}
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    //fill buffer with next audio to be played in the mixer
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);
}

void MainComponent::resized()
{
    //setting size and bounds of the 2 decks and playlist component
    deckGUI1.setBounds(0, 0, getWidth()/2 , getHeight() / 1.6);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight()/1.6);
    playlistComponent.setBounds(0, getHeight() / 1.6, getWidth(), getHeight() / 2);
}















//#include "MainComponent.h"
//
////==============================================================================
//MainComponent::MainComponent()
//{
//    setSize (600, 400);
//
//    addAndMakeVisible(playButton);
//    addAndMakeVisible(volSlider);
//}
//
//MainComponent::~MainComponent()
//{
//}
//
////==============================================================================
//void MainComponent::paint (juce::Graphics& g)
//{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setFont (juce::Font (16.0f));
//    g.setColour (juce::Colours::white);
//    g.drawText ("Hello from Singapore!", getLocalBounds(), juce::Justification::centred, true);
//}
//
//void MainComponent::resized()
//{
//    // This is called when the MainComponent is resized.
//    // If you add any child components, this is where you should
//    // update their positions.
//    double rowH = getHeight() / 5;
//    playButton.setBounds(0, 0, getWidth(), rowH);
//    volSlider.setBounds(0, rowH, getWidth(), rowH);
//}
