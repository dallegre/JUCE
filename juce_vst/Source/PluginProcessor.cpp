/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Juce_vstAudioProcessor::Juce_vstAudioProcessor()
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

Juce_vstAudioProcessor::~Juce_vstAudioProcessor()
{
}

//==============================================================================
const String Juce_vstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juce_vstAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juce_vstAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Juce_vstAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juce_vstAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juce_vstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Juce_vstAudioProcessor::setCurrentProgram (int index)
{
}

const String Juce_vstAudioProcessor::getProgramName (int index)
{
    return {};
}

void Juce_vstAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Juce_vstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	volumeVal = 0.5;
	wetVal = 0.5;
	feedbackVal = 0.5;
	delayVal = 0.5;
	delay.prepareToPlay();
}

void Juce_vstAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juce_vstAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Juce_vstAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {

		buffer.clear(i, 0, buffer.getNumSamples());

	}

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		// ..do something to the data...
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
			
			//this is how you get data from the inputs
			float data = buffer.getSample(channel, sample);
			
			//how to synthesize noise
			//data = random.nextFloat() * 0.25f - 0.125f;
			data *= volumeVal;

			//apply a delay
			delay.updateIndex(delayVal, channel);	
			delay.write(channel,(data + feedbackVal * delay.read(channel)));
			data = data + wetVal * delay.read(channel);

			buffer.setSample(channel, sample, data);

			delay.clearUnused(channel);

		}
	}
}

//==============================================================================
bool Juce_vstAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Juce_vstAudioProcessor::createEditor()
{
    return new Juce_vstAudioProcessorEditor (*this);
}

//==============================================================================
void Juce_vstAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Juce_vstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_vstAudioProcessor();
}
