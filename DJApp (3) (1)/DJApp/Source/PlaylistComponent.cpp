/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Mar 2023 7:08:27pm
    Author:  andre

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
//==============================================================================

PlaylistComponent::PlaylistComponent(DJAudioPlayer* _playerForParsingMetaData,
                                    DeckGUI* _deckGUI1,
                                    DeckGUI* _deckGUI2)
    :
    playerForParsingMetaData(_playerForParsingMetaData),
    deckGUI1(_deckGUI1),
    deckGUI2(_deckGUI2)
{
    //adding column headers and size
    tableComponent.getHeader().addColumn("Track title",1,500);
    tableComponent.getHeader().addColumn("Duration",2, 100);
    tableComponent.getHeader().addColumn("Delete",3,150);
    tableComponent.setModel(this);

    //making buttons and table visible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playDeck1Button);
    addAndMakeVisible(playDeck2Button);

    //adding listener for buttons
    loadButton.addListener(this);
    playDeck1Button.addListener(this);
    playDeck2Button.addListener(this);

    //setting colour for the buttons
    juce::Colour buttonColour = juce::Colours::aqua;
    juce::Colour buttonColour1 = juce::Colours::lightpink;
    juce::Colour buttonColour2 = juce::Colours::aquamarine;
    loadButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    loadButton.setColour(juce::TextButton::textColourOffId, Colours::black);
    playDeck1Button.setColour(juce::TextButton::buttonColourId, buttonColour1);
    playDeck1Button.setColour(juce::TextButton::textColourOffId, Colours::black);
    playDeck2Button.setColour(juce::TextButton::buttonColourId, buttonColour2);
    playDeck2Button.setColour(juce::TextButton::textColourOffId, Colours::black);
    //loading the music table
    loadMusic();
}

PlaylistComponent::~PlaylistComponent()
{
    //calling updateLibrary function
    updateLibrary();
    tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    //ssetting colour and styles for the playlist component
    g.fillAll(juce::Colours::black);  // fill the background
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, 0, getWidth(), getHeight()/2.3);
    loadButton.setBounds(0, (getHeight()/8)*3.5 , getWidth()/3, getHeight() / 3.2);
    playDeck1Button.setBounds(getWidth() / 3, (getHeight()/8)*3.5 , getWidth()/3, getHeight() / 3.2);
    playDeck2Button.setBounds(getWidth() / 3 + getWidth() / 3, (getHeight()/8)*3.5 , getWidth()/3, getHeight() / 3.2);
}

int PlaylistComponent::getNumRows()
{
    //return size of the songs vector
    return vectorSongs.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g,
                                        int rowNumber,
                                        int width,
                                        int height,
                                        bool rowIsSelected)
{
    if (rowIsSelected) 
    {
        //colour for selected row
        g.fillAll(Colours::orange);
    }
    else{
        //colour of unselected row
        g.fillAll(Colours::black);
    }
}

void PlaylistComponent::paintCell(Graphics & g,
                                int rowNumber,
                                int columnId,
                                int width,
                                int height,
                                bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        //setting colour and drawing text in rows
        g.setColour(Colours::white);
        if (columnId == 1)
        {
            g.drawText(vectorSongs[rowNumber].title,
                        2,
                        0,
                        width - 4,
                        height,
                        juce::Justification::centredLeft,
                        true);
        }
        if (columnId == 2)
        {
            g.drawText(vectorSongs[rowNumber].length,
                        2,
                        0,
                        width - 4,
                        height,
                        juce::Justification::centred,
                        true
            );
        }
        tableComponent.updateContent();
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                            int columnId,
                                            bool isRowSelected,
                                            Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            //initialising delete button
            TextButton* btn = new juce::TextButton{ "Delete" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadButton)
    {  
        if (fChooser.browseForMultipleFilesToOpen())
        {
            for (const File& file : fChooser.getResults())
            {
                String filename = {file.getFileNameWithoutExtension()};
                if (!fileInsideSongLibrary(filename))
                {
                    //adding song to library when load button is pressed
                    addSongToLibrary(file);
                }
            }
        }
        tableComponent.updateContent();
    }
    else if (button == &playDeck1Button)
    {
        //loading the selected song into deck 1
        loadSelectedSong(deckGUI1);
    }
    else if (button == &playDeck2Button)
    {
        //loading the selected song into deck 2
        loadSelectedSong(deckGUI2);
    }
    else
    {
        //deleting the song
        int id = std::stoi(button->getComponentID().toStdString());
        deleteSong(id);
        tableComponent.updateContent();
    }
}

String PlaylistComponent::calculateLength(double lengthInSeconds)
{
    //getting the length of song in seconds
    int roundedSeconds = int(std::round(lengthInSeconds));
    String minutes = std::to_string(roundedSeconds / 60);
    String seconds = std::to_string(roundedSeconds % 60);

    if (seconds.length() < 2)
    {
        seconds = seconds.paddedLeft('0', 2);
    }
    String value = juce::String{ minutes + ":" + seconds };
    return String{ minutes + ":" + seconds };
}

void PlaylistComponent::loadMusic()
{
    std::ifstream myLibrary("songsPlaylist.csv");
    std::string fpath;
    std::string length;

    if (myLibrary.is_open())
    {
        //pushing song into songs vector
        while (getline(myLibrary, fpath, ',')) {
            File file{ fpath };
            SongLibrary newSong{ file };

            getline(myLibrary, length);
            newSong.length = length;
            vectorSongs.push_back(newSong);
        }
    }
    myLibrary.close();
}

void PlaylistComponent::addSongToLibrary(File file)
{
    //adding song to song library
    SongLibrary newSong{ file };
    playerForParsingMetaData->loadURL(URL{ file });
    double timeInSeconds = playerForParsingMetaData->getLengthInSeconds();
    newSong.length = calculateLength(timeInSeconds);
    vectorSongs.push_back(newSong);
}

bool PlaylistComponent::fileInsideSongLibrary(String fileNameWithoutExtension)
{
    //when song is inside library
    return (std::find(vectorSongs.begin(),vectorSongs.end(), fileNameWithoutExtension) != vectorSongs.end());
}

void PlaylistComponent::updateLibrary()
{
    std::ofstream myLibrary("songsPlaylist.csv");

    for (SongLibrary& music :vectorSongs)
    {
        myLibrary << music.file.getFullPathName() << "," << music.length << "\n";
    }
}

void PlaylistComponent::loadSelectedSong(DeckGUI* deck)
{
    int selectedRow{tableComponent.getSelectedRow()};
    if (selectedRow != -1)
    {
        //loading the url of selected song from vector songs array
        deck->loadURL(vectorSongs[selectedRow].URL);
    }
    else
    {
        //writing error message when no song selected
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Adding a song to Deck:", "First select a song to add to deck ","Ok",
            nullptr
        );
    }
}

void PlaylistComponent::deleteSong(int id)
{
    //deleting song
    vectorSongs.erase(vectorSongs.begin() + id);
}