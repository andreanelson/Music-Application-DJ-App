/*
  ==============================================================================

    SongLibrary.h
    Created: 13 Mar 2023 1:22:22am
    Author:  andre

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
//==============================================================================

class SongLibrary
{
public:
    SongLibrary(File _file);
    // initialising theSongLibrary variable
    File file;
    URL URL;
    String title;
    String length;
    //comparing each songs title
    bool operator==(const String& other) const;
};