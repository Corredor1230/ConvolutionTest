/*
  ==============================================================================

    AudioFileLoader.cpp
    Created: 3 Nov 2023 12:22:38am
    Author:  User

  ==============================================================================
*/

#include "AudioFileLoader.h"

void AudioFileLoader::loadAudioFile(std::function<void> loadFile)
{
    chooser = std::make_unique<juce::FileChooser>(loaderWindowText,
        juce::File{},
        "*");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles
        | juce::FileBrowserComponent::canSelectDirectories;

    chooser->launchAsync(chooserFlags, [this, loadFile](const juce::FileChooser& fc)
        {
            juce::File result = fc.getResult();
            if (result.getFileExtension() == ".wav" || result.getFileExtension() == ".flac"
                || result.getFileExtension() == ".aiff" || result.getFileExtension() == ".mp3")
            {
                loadedFile = result;
                loadFile;
            }
        });
}

juce::File AudioFileLoader::getFile()
{
    if (loadedFile.exists())
        return loadedFile;
    else
        return juce::File{};
}

void AudioFileLoader::setStereo(bool stereo)
{
    stereoState = stereo;
}

bool AudioFileLoader::isStereo()
{
    return stereoState;
}

void AudioFileLoader::setLoaderWindowText(juce::String text)
{
    loaderWindowText = text;
}