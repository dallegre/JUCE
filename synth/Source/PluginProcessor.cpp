/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

#define UPSAMPLING 32

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
    
    noteOn = 0;
    monoNoteOn = 0.0f;
    
    env.prepareToPlay();
    env.setSpeed(1.0f);
    
    osc[0].prepareToPlay();
    osc[1].prepareToPlay();
    osc2[0].prepareToPlay();
    osc2[1].prepareToPlay();
    filter[0].prepareToPlay();
    filter[1].prepareToPlay();
    
    ampSmoothing[0].prepareForPlay();
    ampSmoothing[1].prepareForPlay();

    freqSmoothing[0].prepareForPlay();
    freqSmoothing[1].prepareForPlay();
    freqSmoothing2[0].prepareForPlay();
    freqSmoothing2[1].prepareForPlay();

    oscVal = 0.5f;
    detVal = 0.5f;
    freqVal = 0.1f;
    qVal = 0.5f;
    envVal = 0.6f;
    speedVal = 0.4f;
    
    freqValScaled = 20000.0f * pow(freqVal, 3.0f);
    envValScaled =  1000.0f *  pow(envVal, 3.0f);
    speedValScaled = (1.0f -  speedVal);
    oscValScaled =   (oscVal - 0.5f) * 70.0f;
    detValScaled =   (detVal - 0.5f) * 7.0f;
    
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
            osc[0].setF(m.getMidiNoteInHertz(noteVal));
            osc[1].setF(m.getMidiNoteInHertz(noteVal));
        }else{
            monoNoteOn = 0.0f;
        }
    }
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumOutputChannels; ++channel){
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample){
            
            filter[channel].setFc((freqValScaled + (envValScaled * pow(env.process(),3.0f)))/UPSAMPLING);
            env.setSpeed(speedValScaled);
            filter[channel].setQ(qVal);
            float freqency =  freqSmoothing[channel].process((exp((noteVal + oscValScaled)/17.31f) + random.nextFloat()*5.0f)/UPSAMPLING);
            float freqency2 = freqSmoothing2[channel].process((exp((noteVal + oscValScaled + detValScaled)/17.31f) + random.nextFloat()*5.0f)/UPSAMPLING);
            osc[channel].setF(freqency);
            osc2[channel].setF(freqency2);
            float monoNoteOn2 = ampSmoothing[channel].process(monoNoteOn);
            
            float data;
            
            for(int i = 0; i < UPSAMPLING; i++){
                data = 30.0f * filter[channel].process(0.05f * osc[channel].process() + 0.05f * osc2[channel].process());
            }
            
            data *= monoNoteOn2;
            
            buffer.setSample(channel, sample, data);
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
    return new SynthAudioProcessorEditor (*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}
