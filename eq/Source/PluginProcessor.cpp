/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "GenericEditor.h"

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

	addParameter(amp1P = new AudioParameterFloat("amp1P", // parameter ID
		"Amp", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(q1P = new AudioParameterFloat("q1P", // parameter ID
		"Q", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(freq1P = new AudioParameterFloat("freq	1P", // parameter ID
		"Freq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.2f)); // default value

	addParameter(amp2P = new AudioParameterFloat("amp2P", // parameter ID
		"Amp", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(q2P = new AudioParameterFloat("q2P", // parameter ID
		"Q", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(freq2P = new AudioParameterFloat("freq2P", // parameter ID
		"Freq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value

	addParameter(amp3P = new AudioParameterFloat("amp3P", // parameter ID
		"Amp", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(q3P = new AudioParameterFloat("q3P", // parameter ID
		"Q", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(freq3P = new AudioParameterFloat("freq3P", // parameter ID
		"Freq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.8f)); // default value

	addParameter(gainP = new AudioParameterFloat("gainP", // parameter ID
		"Gain", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value

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

        amp1ValScaled  = (amp1P->get() - 0.5f);
        amp2ValScaled  = (amp2P->get() - 0.5f);
		amp3ValScaled =  (amp3P->get() - 0.5f);
        freq1ValScaled = 10000.0f * pow(freq1P->get(), 3.0f);
        freq2ValScaled = 10000.0f * pow(freq2P->get(), 3.0f);
		freq3ValScaled = 10000.0f * pow(freq3P->get(), 3.0f);
        for(int i = 0; i < 2; i++){
            filter1[i].prepareToPlay();
            filter1[i].setSamplingFreq(sampleRate);
            filter2[i].prepareToPlay();
            filter2[i].setSamplingFreq(sampleRate);
			filter3[i].prepareToPlay();
            filter3[i].setSamplingFreq(sampleRate);
            filter1[i].setFc(freq1ValScaled, amp1ValScaled, q1P->get());
            filter2[i].setFc(freq2ValScaled, amp2ValScaled, q2P->get());
			filter3[i].setFc(freq3ValScaled, amp3ValScaled, q3P->get());
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
		//try to find a more efficient way to do this.
		amp1ValScaled = (amp1P->get() - 0.5f);
		amp2ValScaled = (amp2P->get() - 0.5f);
		amp3ValScaled = (amp3P->get() - 0.5f);
		freq1ValScaled = 10000.0f * pow(freq1P->get(), 3.0f);
		freq2ValScaled = 10000.0f * pow(freq2P->get(), 3.0f);
		freq3ValScaled = 10000.0f * pow(freq3P->get(), 3.0f);
		filter1[channel].setFc(freq1ValScaled, amp1ValScaled, q1P->get());
		filter2[channel].setFc(freq2ValScaled, amp2ValScaled, q2P->get());
		filter3[channel].setFc(freq3ValScaled, amp3ValScaled, q3P->get());

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            data = buffer.getSample(channel, sample);
            data = filter1[channel].process(data);
            data = filter2[channel].process(data);
			data = filter3[channel].process(data);
			data *= gainP->get();
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
	return new GenericEditor(*this);
}

//==============================================================================
void EqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	MemoryOutputStream oStream(destData, true);
	oStream.writeFloat(*amp1P);
	oStream.writeFloat(*q1P);
	oStream.writeFloat(*freq1P);
	oStream.writeFloat(*amp2P);
	oStream.writeFloat(*q2P);
	oStream.writeFloat(*freq2P);
	oStream.writeFloat(*amp3P);
	oStream.writeFloat(*q3P);
	oStream.writeFloat(*freq3P);
	oStream.writeFloat(*gainP);
}

void EqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	MemoryInputStream iStream(data, static_cast<size_t> (sizeInBytes), false);
	*amp1P =  iStream.readFloat();
	*q1P =    iStream.readFloat();
	*freq1P = iStream.readFloat();
	*amp2P =  iStream.readFloat();
	*q2P =    iStream.readFloat();
	*freq2P = iStream.readFloat();
	*amp3P =  iStream.readFloat();
	*q3P =    iStream.readFloat();
	*freq3P = iStream.readFloat();
	*gainP =  iStream.readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqAudioProcessor();
}
