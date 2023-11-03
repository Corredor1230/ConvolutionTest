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

    juce::File loadAudioFile();
    void loadFileForConvolution(juce::dsp::Convolution& conv);
    void setStereo(bool stereoState);
    bool isStereo();

    void setLoaderWindowText(juce::String& text);

private:

    bool stereoState = true;
    juce::File loadedFile;
    juce::String loaderWindowText = "Choose your file";
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;

};