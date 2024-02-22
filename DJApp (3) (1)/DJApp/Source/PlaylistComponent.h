/*
  ==============================================================================

    PlaylistComponent.h
    Created: 7 Mar 2023 7:08:27pm
    Author:  andre

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <fstream>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "SongLibrary.h"
#include "DeckGUI.h"
//==============================================================================

class PlaylistComponent  : public Component,
                          public TableListBoxModel,
                          public Button::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* _playerMetaData,
                    DeckGUI* _deckGUI1,
                    DeckGUI* _deckGUI2);
    ~PlaylistComponent() override;
    //implement paint and resize functions
    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;
    //implement row painting function
    void paintRowBackground(Graphics &,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected) override;
    //implement cell painting function
    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                        int columnId,
                        bool isRowSelected,
                        Component *existingComponentToUpdate) override;
    //implement button clicked function
    void buttonClicked(Button* button) override;
    //implement load music function
    void loadMusic();
    //implement function to update library for new songs
    void updateLibrary();
    //implement function to add songs to library
    void addSongToLibrary(juce::File file);
    //implement loading selected song function
    void loadSelectedSong(DeckGUI* deck);
    //implement delete song function
    void deleteSong(int id);
    //implement boolean variable if song is in library or not
    bool fileInsideSongLibrary(juce::String fileNameWithoutExtension);
    //implement string of song length
    String calculateLength(double lengthInSeconds);


private:
    //creating private table and buttons
    TableListBox tableComponent;
    TextButton loadButton{ "Load Song to Track Library" };
    TextButton playDeck1Button{ "Play on Deck 1" };
    TextButton playDeck2Button{ "Play on Deck 2" };
    std::vector<SongLibrary> vectorSongs;

    FileChooser fChooser{ "Select a file..." };

    //DJAudioPlayer* playerForParsingMetaData;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
