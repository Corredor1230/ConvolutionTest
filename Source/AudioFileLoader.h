/*
  ==============================================================================

    AudioFileLoader.h
    Created: 3 Nov 2023 12:22:38am
    Author:  User

  ==============================================================================
*/

#pragma once

#include<JuceHeader.h>

class AudioFileLoader
{
public:
    AudioFileLoader() {};
    ~AudioFileLoader() {};

    void loadAudioFile();
    void setStereo(bool stereoState);
    bool isStereo();
    juce::File getFile();

    void setLoaderWindowText(juce::String text);
    juce::File loadedFile;

    bool stereoState = true;
    juce::String loaderWindowText = "Choose your file";
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
private:



};