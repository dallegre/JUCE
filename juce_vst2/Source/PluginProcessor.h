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
	float volumeVal;
	float wetVal;
	float feedbackVal;
	float delayVal;
	Random random;
	//just for fun
	//const int DELAYSIZE = 2.0 * int(juce::AudioProcessor::getSampleRate());
	Delay delay;

private:
    //==============================================================================
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_vst2AudioProcessor)
};
