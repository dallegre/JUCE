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
	dryVal = 1.0f;
	wetVal = 0.5f;
	timeVal = 0.9f;

	feedbackVal = 0.0f;
	delayVal = 0.6f;
	oscAmtVal = 0.4f;
	oscFreqVal = 0.1f;
	
	feedback2Val = 0.7f;
	delay2Val = 0.3f;
	oscAmt2Val = 0.5f;
	oscFreq2Val = 0.3f;

	feedback[0] = 0.0f;
	feedback[1] = 0.0f;
	
	delay.prepareToPlay();
	delay2.prepareToPlay();
	delay3.prepareToPlay();
	delay4.prepareToPlay();

	damping.setFc(3000.0f / (48000));
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

void Juce_vstAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int totalNumInputChannels = getTotalNumInputChannels();
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

			//apply a delay
			float oscAmtValScaled =   100.0f * oscAmtVal;				//amount in samples of modulation
			float oscFreqValScaled =  10.0f * oscFreqVal;				//frequency (roughly) of modulation
			float oscAmtVal2Scaled =  100.0f * oscAmt2Val;				//amount in samples of modulation
			float oscFreqVal2Scaled = 10.0f * oscFreq2Val;				//frequency (roughly) of modulation
			float timeValScaled =     1.0f  * timeVal;

			float feedback1, feedback2, feedback3, feedback4;
			float apout1, apout2, apout3, apout4;

			data *= 0.125f;

			//first two allpass filters are controlled by the user
			delay.updateIndex(delayVal, oscAmtValScaled, oscFreqValScaled, channel);
			feedback1 = data + feedbackVal * delay.read(channel);
			delay.write(channel, (feedback1 + feedback[channel] * timeValScaled));
			apout1 = delay.read(channel) - feedbackVal * feedback1;
			
			delay2.updateIndex(delay2Val, oscAmtVal2Scaled, oscFreqVal2Scaled, channel);
			feedback2 = apout1 + feedback2Val * delay2.read(channel);
			delay2.write(channel, feedback2);
			apout2 = delay2.read(channel) - feedback2Val * feedback2;
			
			//just do arbitrary constants for the other 2 allpass filters.
			delay3.updateIndex(.83, 70.0f, 0.3f, channel);
			feedback3 = apout2 + 0.8f * delay3.read(channel);
			delay3.write(channel, feedback3); 
			apout3 = delay3.read(channel) - 0.8f * feedback3;

			delay4.updateIndex(.65, 55.0f, 0.6f, channel);
			feedback4 = apout3 + 0.2f * delay4.read(channel);
			delay4.write(channel, feedback4);
			apout4 = delay4.read(channel) - 0.2f * feedback4;
			
			feedback[channel] = apout4;
			
			//data = dryVal * data*4.0f - wetVal * (apout4*2.0f + apout3 * apout2/2.0f + apout1/3.0f);

			data = dryVal * data*8.0f - wetVal * (apout3*8.0f);

			buffer.setSample(channel, sample, data);

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
