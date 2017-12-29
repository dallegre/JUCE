/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "DSP.h"

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class EqAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    EqAudioProcessor();
    ~EqAudioProcessor();

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
    
    float amp1ValScaled, amp2ValScaled, amp3ValScaled,
		freq1ValScaled, freq2ValScaled, freq3ValScaled, 
		data;
    Peaking filter1[2], filter2[2], filter3[2];
    int prepareToPlayDone = 0;

private:

	AudioParameterFloat* amp1P;
	AudioParameterFloat* amp2P;
	AudioParameterFloat* amp3P;
	AudioParameterFloat* q1P;
	AudioParameterFloat* q2P;
	AudioParameterFloat* q3P;
	AudioParameterFloat* freq1P;
	AudioParameterFloat* freq2P;
	AudioParameterFloat* freq3P;
	AudioParameterFloat* gainP;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqAudioProcessor)
};
