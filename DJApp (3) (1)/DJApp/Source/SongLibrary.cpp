/*
  ==============================================================================

    SongLibrary.cpp
    Created: 13 Mar 2023 1:22:22am
    Author:  andre

  ==============================================================================
*/
#include "SongLibrary.h"
#include <filesystem>
//==============================================================================

SongLibrary::SongLibrary(File _file)
                :
                file(_file), 
                title(_file.getFileNameWithoutExtension()), 
                URL(juce::URL{ _file })
{
  
}

bool SongLibrary::operator==(const String& other) const
{
    return title == other;
}