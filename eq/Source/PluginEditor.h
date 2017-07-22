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
class EqAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    EqAudioProcessorEditor (EqAudioProcessor&);
    ~EqAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EqAudioProcessor& processor;
    
    Slider amp1Slider, freq1Slider, q1Slider, 
		amp2Slider, freq2Slider, q2Slider,
		amp3Slider, freq3Slider, q3Slider,
		gainSlider;
    
	void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqAudioProcessorEditor)
};
