/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Allpass.h"

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class ReverbAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ReverbAudioProcessor();
    ~ReverbAudioProcessor();

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

	float feedback[2];

	AudioParameterFloat* dryP;
	AudioParameterFloat* wetP;
	AudioParameterFloat* timeP;
	AudioParameterFloat* dampP;

	float feedbackVal;
	float delayVal;
	float oscAmtVal;
	float oscFreqVal;

	float feedback2Val;
	float delay2Val;
	float oscAmt2Val;
	float oscFreq2Val;

	float oscAmtValScaled;				//amount in samples of modulation
	float oscFreqValScaled;				//frequency (roughly) of modulation
	float oscAmtVal2Scaled;				//amount in samples of modulation
	float oscFreqVal2Scaled;			//frequency (roughly) of modulation
	float timeValScaled;
	float dampValScaled;

	int donePrepareToPlay;

	Random random;
	//just for fun
	//const int DELAYSIZE = 2.0 * int(juce::AudioProcessor::getSampleRate());
	Allpass allpass[2], allpass2[2], allpass3[2], allpass4[2];
	OnePoleLp damping[2];

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessor)
};
