/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Delay.h"

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class Juce_vstAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Juce_vstAudioProcessor();
    ~Juce_vstAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	float feedback;

	float dryVal;
	float wetVal;
	float timeVal;

	float feedbackVal;
	float delayVal;
	float oscAmtVal;
	float oscFreqVal;

	float feedback2Val;
	float delay2Val;
	float oscAmt2Val;
	float oscFreq2Val;

	Random random;
	//just for fun
	//const int DELAYSIZE = 2.0 * int(juce::AudioProcessor::getSampleRate());
	Delay delay, delay2, delay3, delay4;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_vstAudioProcessor)
};
