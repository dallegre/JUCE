/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FileplayerAudioProcessor::FileplayerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FileplayerAudioProcessor::~FileplayerAudioProcessor()
{
}

//==============================================================================
const String FileplayerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FileplayerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FileplayerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double FileplayerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FileplayerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FileplayerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FileplayerAudioProcessor::setCurrentProgram (int index)
{
}

const String FileplayerAudioProcessor::getProgramName (int index)
{
    return {};
}

void FileplayerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FileplayerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void FileplayerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FileplayerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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


void FileplayerAudioProcessor::getNextAudioBlock(AudioSourceChannelInfo &bufferToFill) {
	if (readerSource == nullptr){
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	transportSource.getNextAudioBlock(bufferToFill);
}

void FileplayerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	//https://forum.juce.com/t/using-the-audiobuffer-tutorial-method-in-processblock/19034/2
	bufferToFill.buffer = &buffer;
	bufferToFill.startSample = 0;
	bufferToFill.numSamples = buffer.getNumSamples();
	getNextAudioBlock(bufferToFill);
}

//==============================================================================
bool FileplayerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FileplayerAudioProcessor::createEditor()
{
    return new FileplayerAudioProcessorEditor (*this);
}

//==============================================================================
void FileplayerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FileplayerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FileplayerAudioProcessor();
}
