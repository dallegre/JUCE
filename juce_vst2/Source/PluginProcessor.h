/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Delay.h"

#define UPSAMPLING 32

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class Juce_vst2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Juce_vst2AudioProcessor();
    ~Juce_vst2AudioProcessor();

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

	//variable to handle the volume control.  Not sure about making it public
	float filterFreqVal;
	float filterQVal;

	float filter2FreqVal;
	float filter2QVal;

	float dryVal;
	float wetVal;
	float feedbackVal;
	float delayVal;
	float oscAmtVal;
	float oscFreqVal;

	float filterFreqScaled, filter2FreqScaled, oscAmtValScaled, oscFreqValScaled;

	int prepareToPlayDone = 0;

	stateVariable svfilter[2], svfilter2[2];

private:

	Random random;

	Delay delay[2];
    gsOsc modOsc;
	float prevSample[2], data, data2, upSamples[UPSAMPLING];
	OnePoleLp freqSmoothing, fcSmoothing, fc2Smoothing;

    //==============================================================================
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_vst2AudioProcessor)
};
