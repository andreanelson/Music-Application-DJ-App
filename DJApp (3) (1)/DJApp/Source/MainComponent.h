
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
//==============================================================================

class MainComponent : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();
    //implement functions for preparing audio files
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    //implement paint and resize functions
    void paint(Graphics& g) override;
    void resized() override;

private:
    //implementing cache to store playlist songs
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{ 100 };
    //implementing deck 1 player
    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    //implementing deck 2 player
    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };
    //implementing audio mixer
    MixerAudioSource mixerSource;
    //implementing playlist funcitons
    DJAudioPlayer playerMetaData{ formatManager };
    PlaylistComponent playlistComponent{&playerMetaData,&deckGUI1,&deckGUI2 };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

