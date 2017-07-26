/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <math.h>


//==============================================================================
EqAudioProcessor::EqAudioProcessor()
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

EqAudioProcessor::~EqAudioProcessor()
{
}

//==============================================================================
const String EqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double EqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EqAudioProcessor::setCurrentProgram (int index)
{
}

const String EqAudioProcessor::getProgramName (int index)
{
    return {};
}

void EqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if(!prepareToPlayDone){
        amp1Val = 0.5f;
        amp2Val = 0.5f;
		amp3Val = 0.5f;
        freq1Val = 0.2f;
        freq2Val = 0.5f;
		freq3Val = 0.8f;
		q1Val = 0.5f;
		q2Val = 0.5f;
		q3Val = 0.5f;
		gainVal = 1.0f;
        amp1ValScaled  = (amp1Val - 0.5f);
        amp2ValScaled  = (amp2Val - 0.5f);
		amp3ValScaled =  (amp3Val - 0.5f);
        freq1ValScaled = 10000.0f * pow(freq1Val, 3.0f);
        freq2ValScaled = 10000.0f * pow(freq2Val, 3.0f);
		freq3ValScaled = 10000.0f * pow(freq3Val, 3.0f);
        for(int i = 0; i < 2; i++){
            filter1[i].prepareToPlay();
            filter1[i].setSamplingFreq(sampleRate);
            filter2[i].prepareToPlay();
            filter2[i].setSamplingFreq(sampleRate);
			filter3[i].prepareToPlay();
            filter3[i].setSamplingFreq(sampleRate);
            filter1[i].setFc(freq1ValScaled, amp1ValScaled, 6.0f);
            filter2[i].setFc(freq2ValScaled, amp2ValScaled, 6.0f);
			filter3[i].setFc(freq3ValScaled, amp3ValScaled, 6.0f);
        }
        prepareToPlayDone = 1;
    }
}

void EqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EqAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            data = buffer.getSample(channel, sample);
            data = filter1[channel].process(data);
            data = filter2[channel].process(data);
			data = filter3[channel].process(data);
			data *= gainVal;
			buffer.setSample(channel, sample, data);
        }
    }
}

//==============================================================================
bool EqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EqAudioProcessor::createEditor()
{
    return new EqAudioProcessorEditor (*this);
}

//==============================================================================
void EqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqAudioProcessor();
}
