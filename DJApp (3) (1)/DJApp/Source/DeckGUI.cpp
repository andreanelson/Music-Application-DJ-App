/*
  ==============================================================================

    DeckGUI.cpp
    Created: 6 Mar 2023 3:48:24pm
    Author:  andre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse
) : player(_player),
waveformDisplay(formatManagerToUse, cacheToUse)
{
    //making the buttons visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(fadeButton);

    //making the sliders visible
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    //making the waveform visible
    addAndMakeVisible(waveformDisplay);

    //adding listeners for all the classes
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    fadeButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    //set range of each slider
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    //set style of each slider
    volSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

    //implementing a timer
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    //stopping timer
    stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
    g.fillAll(juce::Colours::black);  //filling the background to be black
    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   //draw an outline around the component

    //specifying the style details of the text
    g.setColour(Colours::white);
    g.setFont(20.0f);
    g.drawText("DJ Deck", getLocalBounds(),
        Justification::centredTop, true);   // draw placeholder text
    g.setFont(14.0f);
    g.drawText("volume", 15, 0, 50, getHeight() / 1.6, Justification::centred, true);
    g.drawText("speed", 0, 0, getWidth()*1.08, getHeight()/1.6 , Justification::centred, true);
    g.drawText("position", 15, 0, 50, getHeight()/1.01 , Justification::centred, true);
    
    //setting colours of the buttons
    playButton.setColour(TextButton::buttonColourId, juce::Colours::yellowgreen);
    stopButton.setColour(TextButton::buttonColourId, juce::Colours::lightsteelblue);
    fadeButton.setColour(TextButton::buttonColourId, juce::Colours::indianred);
}

void DeckGUI::resized()
{
    //setting the bounds for each object
    double rowH = getHeight() / 8;
    playButton.setBounds(0, rowH/1.7, getWidth()/3, rowH);
    stopButton.setBounds(getWidth() / 3, rowH/1.7, getWidth()/3, rowH);
    fadeButton.setBounds(getWidth()/3+ getWidth() / 3, rowH/1.7, getWidth() / 3, rowH);
    volSlider.setBounds(0, rowH * 2, getWidth()/2, rowH*2);
    speedSlider.setBounds(getWidth() / 2, rowH * 2, getWidth()/2, rowH*2);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH*2 );
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        //calling start function to start audio when play is hit
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        //calling stop function to stop audio when stop is hit
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
        //choosing and loading file to player when load button is hit
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                player->loadURL(URL{ chooser.getResult() });
                // loading url to waveformDisplay as well
                waveformDisplay.loadURL(URL{ chooser.getResult() });
            });
    }
    if (button == &fadeButton)
    {
        //calling the fadeAudio function when fade button is hit
        player->fadeAudio();
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        //changing gain value based on slider value 
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        //changing speed value based on slider value 
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        //changing position value based on slider value 
        player->setPositionRelative(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    //is file being dragged
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    //when file dropped,loadURL in player
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{ File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    //setting position in the waveform of current audio position
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadURL(URL audioURL)
{
    //loading the player and waveform display using the url
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}



