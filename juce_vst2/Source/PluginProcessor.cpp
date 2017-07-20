/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


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
		
		filterFreqVal =  0.0f;
		filterQVal =     0.2f;

		filter2FreqVal = 1.0f;
		filter2QVal =    0.2f;
        
        filterFreqScaled =  10000.0f *  pow(filterFreqVal,  3.0);
        filter2FreqScaled = 20000.0f *  pow(filter2FreqVal, 3.0);

		dryVal =      1.0f;
		wetVal =      0.0f;
		feedbackVal = 0.5f;
		delayVal =    0.5f;
		oscAmtVal =   0.7f;
		oscFreqVal =  0.3f;
        
        oscAmtValScaled =  50.0f   * oscAmtVal;
        oscFreqValScaled = 2.0f *    oscFreqVal;

		for (int i = 0; i < 2; i++) {
			delay[i].prepareToPlay();
			svfilter[i].prepareToPlay();
			svfilter2[i].prepareToPlay();
			svfilter[i].setFc(filterFreqScaled, UPSAMPLING);
			svfilter[i].setQ(1.0f);
			svfilter2[i].setFc(filter2FreqScaled, UPSAMPLING);
			svfilter2[i].setQ(1.0f);
            prevSample[i] = 0.0f;
		}
        
        fcSmoothing.prepareToPlay();
        fc2Smoothing.prepareToPlay();
		fcSmoothing.setFc(1.0f);
		fc2Smoothing.setFc(1.0f);
        
        data =  0.0f;
        data2 = 0.0f;
        
		prepareToPlayDone = 1;

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
            data = buffer.getSample(channel, sample);

			//how to synthesize noise
			//data = random.nextFloat() * 0.25f - 0.125f;
            
			//upsampling loop
			for (int i = 0; i < UPSAMPLING - 1; i++) {
				data2 = (prevSample[channel] * float(((float(UPSAMPLING) - 1) - i)/(float(UPSAMPLING) - 1))) + (data * float(i/(float(UPSAMPLING) - 1)));
                data2 = svfilter[channel].processHP(data2);
                upSamples[i] = data2;
            }
            //upsample by half the amount for the highpass (it's high cutoff operation is not nearly as critical as nothing will really be getting through as opposed to the entire signal
            for(int i = 0; i < UPSAMPLING; i += 2){
                data2 = svfilter2[channel].processLP(data2);
			}

			//apply a delay
			delay[channel].updateIndex(delayVal, oscAmtValScaled, oscFreqValScaled, channel);
			delay[channel].write((data2 + feedbackVal * delay[channel].read()));
			data2 = dryVal * data2 + wetVal * delay[channel].read();

			buffer.setSample(channel, sample, data2);

			prevSample[channel] = data;

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
