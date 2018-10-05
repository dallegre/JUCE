/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "GenericEditor.h"


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

	prepareToPlayDone = 0;

	addParameter(freq1P = new AudioParameterFloat("freq1P", // parameter ID
		"HPfreq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.0f)); // default value
	addParameter(q1P = new AudioParameterFloat("q1P", // parameter ID
		"HPQ", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.2f)); // default value

	addParameter(freq2P = new AudioParameterFloat("freq2P", // parameter ID
		"LPfreq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		1.0f)); // default value
	addParameter(q2P = new AudioParameterFloat("q2P", // parameter ID
		"LPQ", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.2f)); // default value

	addParameter(dryP = new AudioParameterFloat("dryP", // parameter ID
		"Dry", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		1.0f)); // default value
	addParameter(wetP = new AudioParameterFloat("wetP", // parameter ID
		"Wet", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.0f)); // default value
	addParameter(delP = new AudioParameterFloat("delP", // parameter ID
		"Delay", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(fbP = new AudioParameterFloat("fbP", // parameter ID
		"Feedback", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value
	addParameter(mamtP = new AudioParameterFloat("mamtP", // parameter ID
		"Mod Amount", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.7f)); // default value
	addParameter(mfreqP = new AudioParameterFloat("mfreqP", // parameter ID
		"Mod Freq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.3f)); // default value

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
        
        oscAmtValScaled =  50.0f   * mamtP->get();
        oscFreqValScaled = 2.0f *    mfreqP->get();

		for (int i = 0; i < 2; i++) {
			delay[i].prepareToPlay(sampleRate);
			svfilter[i].prepareToPlay();
            svfilter[i].setSamplingFreq(sampleRate);
			svfilter2[i].prepareToPlay();
            svfilter2[i].setSamplingFreq(sampleRate);
			svfilter[i].setFc(filterFreqScaled, UPSAMPLING);
			svfilter[i].setQ(0.0f);
			svfilter2[i].setFc(filter2FreqScaled, UPSAMPLING);
			svfilter2[i].setQ(0.0f);
            prevSample[i] = 0.0f;
		}
        
        fcSmoothing.prepareToPlay();
        fc2Smoothing.prepareToPlay();
        fcSmoothing.setSamplingFreq(sampleRate);
        fc2Smoothing.setSamplingFreq(sampleRate);
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

		//again put this in a very inefficient place...
		filterFreqScaled = 10000.0f *  pow(freq1P->get(), 3.0);
		filter2FreqScaled = 20000.0f *  pow(freq2P->get(), 3.0);

		oscAmtValScaled = 100.0f * mamtP->get();
		oscFreqValScaled = 4.0f * mfreqP->get();

		svfilter[channel].setFc(filterFreqScaled, UPSAMPLING);
		svfilter[channel].setQ(q1P->get());
		svfilter2[channel].setFc(filter2FreqScaled, UPSAMPLING);
		svfilter2[channel].setQ(q2P->get());

		// ..do something to the data...
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

			//this is how you get data from the inputs
            data = buffer.getSample(channel, sample);

			//how to synthesize noise
			//data = random.nextFloat() * 0.25f - 0.125f;

			data = data / 100.0f;
            
			//upsampling loop
			for (int i = 0; i < UPSAMPLING - 1; i++) {
				data2 = (prevSample[channel] * float(((float(UPSAMPLING) - 1) - i)/(float(UPSAMPLING) - 1))) + (data * float(i/(float(UPSAMPLING) - 1)));
                data2 = svfilter[channel].processHP(data2);
                upSamples[i] = data2;
            }
            //upsample by half the amount for the highpass (it's high cutoff operation is not nearly as critical as nothing will really be getting through as opposed to the entire signal
            for(int i = 0; i < UPSAMPLING; i += 2){
                data2 = svfilter2[channel].processLP(upSamples[i]);
			}

			//apply a delay
			delay[channel].updateIndex(delP->get(), oscAmtValScaled, oscFreqValScaled, channel);
			delay[channel].write((data2 + fbP->get() * delay[channel].read()));
			data2 = dryP->get() * data2 + wetP->get() * delay[channel].read();

			data2 = data2 * 100.0f;

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
	return new GenericEditor(*this);
}

//==============================================================================
void Juce_vst2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	MemoryOutputStream oStream(destData, true);
	oStream.writeFloat(*freq1P);
	oStream.writeFloat(*q1P);
	oStream.writeFloat(*freq2P);
	oStream.writeFloat(*q2P);
	oStream.writeFloat(*wetP);
	oStream.writeFloat(*dryP);
	oStream.writeFloat(*delP);
	oStream.writeFloat(*fbP);
	oStream.writeFloat(*mamtP);
	oStream.writeFloat(*mfreqP);


}

void Juce_vst2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	MemoryInputStream iStream(data, static_cast<size_t> (sizeInBytes), false);
	*freq1P = iStream.readFloat();
	*q1P =    iStream.readFloat();
	*freq2P = iStream.readFloat();
	*q2P =    iStream.readFloat();
	*wetP =   iStream.readFloat();
	*dryP =   iStream.readFloat();
	*delP =   iStream.readFloat();
	*fbP =    iStream.readFloat();
	*mamtP =  iStream.readFloat();
	*mfreqP = iStream.readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_vst2AudioProcessor();
}
