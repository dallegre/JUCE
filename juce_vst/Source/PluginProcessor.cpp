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
	dampVal = 0.5f;

	feedbackVal = 0.5f;
	delayVal = 0.6f;
	oscAmtVal = 0.4f;
	oscFreqVal = 0.1f;
	
	feedback2Val = 0.7f;
	delay2Val = 0.3f;
	oscAmt2Val = 0.5f;
	oscFreq2Val = 0.3f;

	oscAmtValScaled = 50.0f * oscAmtVal;				//amount in samples of modulation
	oscFreqValScaled = 5.0f * oscFreqVal;				//frequency (roughly) of modulation
	oscAmtVal2Scaled = 50.0f * oscAmt2Val;				//amount in samples of modulation
	oscFreqVal2Scaled = 5.0f * oscFreq2Val;				//frequency (roughly) of modulation
	timeValScaled = 1.0f * timeVal;
	dampValScaled = 16000.0f * pow(dampVal, 2.0f) / 48000;

	feedback[0] = 0.0f;
	feedback[1] = 0.0f;
	
	allpass[0].prepareToPlay();
	allpass2[0].prepareToPlay();
	allpass3[0].prepareToPlay();
	allpass4[0].prepareToPlay();
	allpass[1].prepareToPlay();
	allpass2[1].prepareToPlay();
	allpass3[1].prepareToPlay();
	allpass4[1].prepareToPlay();

	damping[0].setFc(dampValScaled);
	damping[1].setFc(dampValScaled);
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

			float feedback1, feedback2, feedback3, feedback4;
			float apout1, apout2, apout3, apout4, scaledIn;

			//scale the input a bit so you don't saturate as you go through the algorithm
			data *= 0.25;
			scaledIn = data * wetVal;

			//first two allpass filters are controlled by the user
			apout1 = allpass[channel].process((scaledIn - feedback[channel] * timeValScaled),
				delayVal, oscAmtValScaled, oscFreqValScaled, feedbackVal);

			apout2 = allpass2[channel].process(apout1, delay2Val, oscAmtVal2Scaled, oscFreqVal2Scaled, feedback2Val);

			//2nd two allpass filters have constant parameters that differ slightly between left and right.
			if (!channel) {
				apout3 = allpass3[channel].process(apout2, .83, 70.0f, 0.3f, 0.8f);
			}else {
				apout3 = allpass3[channel].process(apout2, .73, 69.0f, 0.3f, 0.8f);
			}

			if (!channel) {
				apout4 = allpass4[channel].process(apout3, .55, 54.0f, 0.6f, 0.2f);
			}else {
				apout4 = allpass4[channel].process(apout3, .65f, 54.0f, 0.6f, 0.2f);
			}

			feedback[channel] = damping[channel].process(apout4);
			//feedback[channel] = apout4;
			
			//Send the taps slightly differently for left and right.  Ping ponging isn't yet working.
			if (!channel) {
				data = (dryVal * data * 4.0f) - (apout4*2.0f + apout3 * apout2 / 2.0f + apout1 / 2.9f);
			}else{
				data = (dryVal * data * 4.0f) - (apout4*2.0f + apout3 * apout2 / 2.1f + apout1 / 3.0f);
			}

			//this is how you can send data to outputs
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
