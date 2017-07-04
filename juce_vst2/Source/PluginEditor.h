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
class Juce_vst2AudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    Juce_vst2AudioProcessorEditor (Juce_vst2AudioProcessor&);
    ~Juce_vst2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Juce_vst2AudioProcessor& processor;

	Slider drySlider;
	Slider wetSlider;
	Slider feedbackSlider;
	Slider delaySlider;
	Slider oscFreqSlider;
	Slider oscAmtSlider;
	void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_vst2AudioProcessorEditor)
};
