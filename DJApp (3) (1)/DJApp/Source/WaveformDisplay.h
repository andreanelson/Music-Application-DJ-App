/*
  ==============================================================================

    WaveformDisplay.h
    Created: 6 Mar 2023 3:49:08pm
    Author:  andre

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
//==============================================================================

class WaveformDisplay : public Component,
    public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();

    void paint(Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

