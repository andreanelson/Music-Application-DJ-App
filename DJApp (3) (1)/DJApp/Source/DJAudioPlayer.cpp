/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 6 Mar 2023 3:44:12pm
    Author:  andre

  ==============================================================================
*/
#include "DJAudioPlayer.h"
//==============================================================================

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                                 : formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //preparing audio for playback
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    //getting following audio data
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    //function to be used when stopping audio
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    //loading file to be played
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        //console output when gain is out of range
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        //setting gain(volume) of audio
        transportSource.setGain(gain);
    }

}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 10.0)
    {
        //console output when speed is out of range
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 10" << std::endl;
    }
    else {
        //setting speed of audio
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    //setting position in the audio file
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        //console output when position is out of range
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        //setting relative position of the playhead in seconds 
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    //start audio
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    //stop audio
    transportSource.stop();
}

void DJAudioPlayer::fadeAudio()
{
    const int sampleRate = 44100; // set sample rate
    const int bufferSize = sampleRate / 10; // buffer size for processing audio
    const double numSteps = transportSource.getLengthInSeconds() * sampleRate / bufferSize;
    const double gainStep = transportSource.getGain() / numSteps;
    double gain = transportSource.getGain();

    for (int i = 0; i < numSteps; ++i)
    {
        AudioBuffer<float>  buffer(2,  bufferSize);
        transportSource.getNextAudioBlock(AudioSourceChannelInfo(&buffer, 0, bufferSize));
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < bufferSize; ++sample)
            {
                channelData[sample] *= gain;
            }
        }
        //set volume 
        transportSource.setGain(gain);
        gain -= gainStep;
        Thread::sleep(100); // wait 100ms before processing the next buffer
        transportSource.stop();
    }
    //stop the audio
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    //get position in the audio file
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
    //get length of audio
    return transportSource.getLengthInSeconds();

}
