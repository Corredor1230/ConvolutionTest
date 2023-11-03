/*
  ==============================================================================

    AudioFileLoader.cpp
    Created: 3 Nov 2023 12:22:38am
    Author:  User

  ==============================================================================
*/

#include "AudioFileLoader.h"

juce::File AudioFileLoader::loadAudioFile()
{
    chooser = std::make_unique<juce::FileChooser>(loaderWindowText,
        juce::File{},
        "*");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            juce::File result = fc.getResult();
            if (loadedFile.getFileExtension() == ".wav" | loadedFile.getFileExtension() == ".flac"
                | loadedFile.getFileExtension() == ".aiff" | loadedFile.getFileExtension() == ".mp3")
            {

                loadedFile = result;

            }
        });

    return loadedFile;
}

void AudioFileLoader::loadFileForConvolution(juce::dsp::Convolution& conv)
{
    chooser = std::make_unique<juce::FileChooser>(loaderWindowText,
        juce::File{},
        "*");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this, &conv](const juce::FileChooser& fc)
        {
            juce::File result = fc.getResult();
            if (loadedFile.getFileExtension() == ".wav" | loadedFile.getFileExtension() == ".flac"
                | loadedFile.getFileExtension() == ".aiff" | loadedFile.getFileExtension() == ".mp3")
            {

                loadedFile = result;
                conv.loadImpulseResponse(result, juce::dsp::Convolution::Stereo::yes,
                                            juce::dsp::Convolution::Trim::yes, 0);
            }
        });
};

void AudioFileLoader::setStereo(bool stereo)
{
    stereoState = stereo;
}

bool AudioFileLoader::isStereo()
{
    return stereoState;
}

void AudioFileLoader::setLoaderWindowText(juce::String& text)
{
    loaderWindowText = text;
}