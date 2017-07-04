/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Juce_vstAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    Juce_vstAudioProcessorEditor (Juce_vstAudioProcessor&);
    ~Juce_vstAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Juce_vstAudioProcessor& processor;

	Slider volumeSlider;
	Slider wetSlider;
	Slider feedbackSlider;
	Slider delaySlider;
	void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_vstAudioProcessorEditor)
};
