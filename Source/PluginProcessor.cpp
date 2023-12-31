/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioLoaderTestAudioProcessor::AudioLoaderTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    loader.setLoaderWindowText("Escoge una respuesta de impulso");
    loader.loadAudioFile();
}

AudioLoaderTestAudioProcessor::~AudioLoaderTestAudioProcessor()
{
}

//==============================================================================
const juce::String AudioLoaderTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioLoaderTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioLoaderTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioLoaderTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioLoaderTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioLoaderTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioLoaderTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioLoaderTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioLoaderTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioLoaderTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioLoaderTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    loadedFile = loader.getFile();

    if (loadedFile.exists())
    {
        convolution.reset();
        convolution.loadImpulseResponse(loadedFile, juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::yes, getSampleRate() * 2);
    }
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    convolution.prepare(spec);

}

void AudioLoaderTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioLoaderTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioLoaderTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block{ buffer };

    if (!loadedFile.exists())
    {
        loadedFile = loader.getFile();
        if (loadedFile.exists())
        {
            convolution.reset();
            convolution.loadImpulseResponse(loadedFile, juce::dsp::Convolution::Stereo::yes,
                juce::dsp::Convolution::Trim::yes, getSampleRate() * 2);
        }
    }

    convolution.process(juce::dsp::ProcessContextReplacing<float>(block));


}

//==============================================================================
bool AudioLoaderTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioLoaderTestAudioProcessor::createEditor()
{
    return new AudioLoaderTestAudioProcessorEditor (*this);
}

//==============================================================================
void AudioLoaderTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioLoaderTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioLoaderTestAudioProcessor();
}
