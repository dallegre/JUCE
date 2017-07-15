/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define UPSAMPLING 32


//==============================================================================
Juce_vst2AudioProcessor::Juce_vst2AudioProcessor()
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

Juce_vst2AudioProcessor::~Juce_vst2AudioProcessor()
{
}

//==============================================================================
const String Juce_vst2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juce_vst2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juce_vst2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Juce_vst2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juce_vst2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juce_vst2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Juce_vst2AudioProcessor::setCurrentProgram (int index)
{
}

const String Juce_vst2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Juce_vst2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Juce_vst2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	if (!prepareToPlayDone) {
		
		filterFreqVal = 0.0f;
		filterQVal = 0.2f;

		filter2FreqVal = 1.0f;
		filter2QVal = 0.2f;

		dryVal = 1.0f;
		wetVal = 0.0f;
		feedbackVal = 0.5f;
		delayVal = 0.5f;
		oscAmtVal = 0.7f;
		oscFreqVal = 0.3f;

		for (int i = 0; i < 2; i++) {
			delay[i].prepareToPlay();
			svfilter[i].prepareToPlay();
			svfilter[i].setFc(1000.0f, UPSAMPLING);
			svfilter[i].setQ(1.0f);
			svfilter2[i].setFc(1000.0f, UPSAMPLING);
			svfilter2[i].setQ(1.0f);
		}

		prevSample[0] = 0.0f;
		prevSample[1] = 0.0f;

		prepareToPlayDone = 1;

		freqSmoothing.setFc(1.0f);
		fcSmoothing.setFc(1.0f);
		fc2Smoothing.setFc(1.0f);

	}

}

void Juce_vst2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juce_vst2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Juce_vst2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

			float currentSample = data;

			//how to synthesize noise
			//data = random.nextFloat() * 0.25f - 0.125f;
			
			//upsampling.  Interpolate between the current sample and the previous sample
			float upSamples[UPSAMPLING];
			for (int i = 0; i < UPSAMPLING; i++) {
				upSamples[i] = (prevSample[channel] * float(((float(UPSAMPLING) - 1) - i)/(float(UPSAMPLING) - 1))) + (data * float(i/(float(UPSAMPLING) - 1)));
			}
			
			//equalization
			filterFreqScaled = 20000.0f *  pow(filterFreqVal, 3.0);
			filterFreqScaled = fcSmoothing.process(filterFreqScaled);
			filter2FreqScaled = 20000.0f *  pow(filter2FreqVal, 3.0);
			filter2FreqScaled = fc2Smoothing.process(filter2FreqScaled);
			svfilter[channel].setFc(filterFreqScaled, UPSAMPLING);
			svfilter[channel].setQ(filterQVal);
			svfilter2[channel].setFc(filter2FreqScaled, UPSAMPLING);
			svfilter2[channel].setQ(filter2QVal);

			//upsampling loop
			for (int i = 0; i < UPSAMPLING - 1; i++) {
				upSamples[i] = svfilter[channel].process(upSamples[i], 2);
				upSamples[i] = svfilter2[channel].process(upSamples[i], 0);
			}
			 
			//do "decimation"
			data = upSamples[UPSAMPLING - 2]; 

			//apply a delay
			float oscAmtValScaled =  50.0f   * oscAmtVal;										//amount in samples of modulation
			float oscFreqValScaled = 2.0f    * freqSmoothing.process(oscFreqVal);				//frequency (roughly) of modulation
			delay[channel].updateIndex(delayVal, oscAmtValScaled, oscFreqValScaled, channel);
			delay[channel].write((data + feedbackVal * delay[channel].read()));
			data = dryVal * data + wetVal * delay[channel].read();

			buffer.setSample(channel, sample, data);

			prevSample[channel] = currentSample;

		}
	}
}

//==============================================================================
bool Juce_vst2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Juce_vst2AudioProcessor::createEditor()
{
    return new Juce_vst2AudioProcessorEditor (*this);
}

//==============================================================================
void Juce_vst2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Juce_vst2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_vst2AudioProcessor();
}
