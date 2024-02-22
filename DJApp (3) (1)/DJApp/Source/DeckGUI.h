/*
  ==============================================================================

    DeckGUI.h
    Created: 6 Mar 2023 3:48:24pm
    Author:  andre

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================

class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~DeckGUI();
    //implement paint and resized functions
    void paint(Graphics&) override;
    void resized() override;

    //implement button clicked function
    void buttonClicked(Button*) override;

    //imlpement function for change in slider value
    void sliderValueChanged(Slider* slider) override;

    //implement functions to identify when file is being dragged and dropped
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    //function for position in audio
    void timerCallback() override;
    //function to loadurl
    void loadURL(juce::URL audioURL);

private:
    //initialising buttons
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    TextButton fadeButton{ "FADE" };
    //initialising sliders
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    //text when choosing file
    FileChooser fChooser{ "Select a file..." };
    //calling waveform display
    WaveformDisplay waveformDisplay;
    //calling the DJAudio player
    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};



