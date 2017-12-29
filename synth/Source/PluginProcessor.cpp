/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "GenericEditor.h"
#include <math.h>

#define UPSAMPLING 64

//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
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
	addParameter(oscP = new AudioParameterFloat("oscP", // parameter ID
		"Tune", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value

	addParameter(detP = new AudioParameterFloat("detP", // parameter ID
		"Detune", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value

	addParameter(ampP = new AudioParameterFloat("ampP", // parameter ID
		"Amp", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value

	addParameter(freqP = new AudioParameterFloat("freqP", // parameter ID
		"Freq", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.2f)); // default value

	addParameter(qP = new AudioParameterFloat("qP", // parameter ID
		"Q", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.5f)); // default value

	addParameter(envP = new AudioParameterFloat("envP", // parameter ID
		"Env", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.9f)); // default value

	addParameter(speedP = new AudioParameterFloat("speedP", // parameter ID
		"Decay", // parameter name
		0.0f,   // mininum value
		1.0f,   // maximum value
		0.6f)); // default value

}

SynthAudioProcessor::~SynthAudioProcessor()
{

}

//==============================================================================
const String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAudioProcessor::setCurrentProgram (int index)
{
}

const String SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	if (!prepareToPlayDone) {
    
        noteOn = 0;
        monoNoteOn = 0.0f;
        
        env.prepareToPlay();
        env.setSampleRate(sampleRate);
        env.setSpeed(1.0f);
        
        osc.prepareToPlay();
        osc2.prepareToPlay();
        filter.prepareToPlay();
        
        ampSmoothing.prepareForPlay();
		driftSmoothing.prepareForPlay();
		driftSmoothing2.prepareForPlay();
		freqSmoothing.prepareForPlay();
        modOsc.prepareForPlay();
        
        modOsc.setSampleRate(sampleRate);
        ampSmoothing.setSampleRate(sampleRate);
        driftSmoothing.setSampleRate(sampleRate);
        driftSmoothing2.setSampleRate(sampleRate);
        freqSmoothing.setSampleRate(sampleRate);
        osc.setSampleRate(sampleRate);
        osc2.setSampleRate(sampleRate);
        filter.setSampleRate(sampleRate);
        
        
        modOsc.setF(0.1f,0.05f);
		ampSmoothing.setFc2(200.0f);			//slow enough to avoid clicks, but fast enough to be snappy
		driftSmoothing.setFc2(0.1f);			//want this to be pretty slow, mimick oscillator drift
		driftSmoothing2.setFc2(0.1f);
		freqSmoothing.setFc2(200.0f);
        
        freqValScaled = 20000.0f * pow(freqP->get(), 3.0f);
        envValScaled =  10000.0f *  pow(envP->get(), 3.0f);
        speedValScaled = pow((1.0f -  speedP->get()),2.0f);
        oscValScaled =   (oscP->get() - 0.5f) * 70.0f;
        detValScaled =   (detP->get() - 0.5f) * 7.0f;
        
        prepareToPlayDone = 1;
    
    }
    
}

void SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SynthAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.

    MidiBuffer Midi;
    int time;
    MidiMessage m;
    
    for(MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);){
        
        //handle monophonic on/off of notes
        if(m.isNoteOn()){
            noteOn++;
        }
        if(m.isNoteOff()){
            noteOn--;
        }
        if(noteOn > 0){
            monoNoteOn = 1.0f;
            env.reset();
            //handle the pitch of the note
            noteVal = m.getNoteNumber();
            osc.setF(m.getMidiNoteInHertz(noteVal));
        }else{
            monoNoteOn = 0.0f;
        }
    }
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumOutputChannels; ++channel){
        
        //just do the synth stuff on one channel.
        if(channel == 0){

			//do this stuff here.  it's terribly inefficient..
			freqValScaled = 20000.0f * pow(freqP->get(), 3.0f);
			envValScaled = 10000.0f *  pow(envP->get(), 3.0f);
			speedValScaled = pow((1.0f - speedP->get()), 2.0f);
			oscValScaled = (oscP->get() - 0.5f) * 70.0f;
			detValScaled = (detP->get() - 0.5f) * 7.0f; 

            for(int sample = 0; sample < buffer.getNumSamples(); ++sample){
                
                filter.setFc(freqSmoothing.process(freqValScaled + (envValScaled * pow(env.process(),3.0f))) / UPSAMPLING);
                env.setSpeed(speedValScaled);
                filter.setQ(qP->get());
				float frequency = noteVal + 24.0f + oscValScaled + modOsc.process(0) + (driftSmoothing.process((random.nextFloat() * 0.5f) - 0.25f) * 20.0f);
                float frequency2 = exp((frequency + detValScaled + (driftSmoothing2.process(random.nextFloat() * 0.5f - 0.25f) * 20.0f)) / 17.31f) / UPSAMPLING;
				frequency = exp(frequency / 17.31f) / UPSAMPLING;
				osc.setF(frequency);
                osc2.setF(frequency2);
                float monoNoteOn2 = ampSmoothing.process(monoNoteOn);
                
                float data;
                
                for(int i = 0; i < UPSAMPLING; i++){
                    data = 20.0f * filter.process(0.1f * osc.process() + ampP->get() * 0.1f * osc2.process());
                }
                
                data *= monoNoteOn2;
                
                buffer.setSample(0, sample, data);
                buffer.setSample(1, sample, data);
            }
        }
    }
}

//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    //return new SynthAudioProcessorEditor (*this);
	return new GenericEditor(*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	MemoryOutputStream oStream(destData, true);
	oStream.writeFloat(*oscP);
	oStream.writeFloat(*detP);
	oStream.writeFloat(*ampP);
	oStream.writeFloat(*freqP);
	oStream.writeFloat(*qP);
	oStream.writeFloat(*envP);
	oStream.writeFloat(*speedP);

	//MemoryOutputStream(destData, true).writeFloat(*speedP);

}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

	MemoryInputStream iStream(data, static_cast<size_t> (sizeInBytes), false);
	*oscP = iStream.readFloat();
	*detP = iStream.readFloat();
	*ampP = iStream.readFloat();
	*freqP = iStream.readFloat();
	*qP = iStream.readFloat();
	*envP = iStream.readFloat();
	*speedP = iStream.readFloat();

	//*oscP =   MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat();

	//*speedP = MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat();

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}
