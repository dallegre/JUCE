/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Juce_vst2AudioProcessorEditor::Juce_vst2AudioProcessorEditor (Juce_vst2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(450, 300);
	setResizable(false, true);

	//the constructor gets called every time you click on the plugin so initialize everything at prepare to play
	//set up parameters for the slider
	drySlider.setSliderStyle(Slider::LinearBarVertical);
	drySlider.setRange(0.0, 1.0, 0.01);
	drySlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	drySlider.setPopupDisplayEnabled(true, this);
	drySlider.setTextValueSuffix(" Dry");
	drySlider.setValue(processor.dryVal);

	wetSlider.setSliderStyle(Slider::LinearBarVertical);
	wetSlider.setRange(0.0, 1.0, 0.01);
	wetSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	wetSlider.setPopupDisplayEnabled(true, this);
	wetSlider.setTextValueSuffix(" Wet");
	wetSlider.setValue(processor.wetVal);

	feedbackSlider.setSliderStyle(Slider::LinearBarVertical);
	feedbackSlider.setRange(0.0, 1.0, 0.01);
	feedbackSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	feedbackSlider.setPopupDisplayEnabled(true, this);
	feedbackSlider.setTextValueSuffix(" Feedback");
	feedbackSlider.setValue(processor.feedbackVal);

	delaySlider.setSliderStyle(Slider::LinearBarVertical);
	delaySlider.setRange(0.0, 1.0, 0.01);
	delaySlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	delaySlider.setPopupDisplayEnabled(true, this);
	delaySlider.setTextValueSuffix(" Delay");
	delaySlider.setValue(processor.delayVal);

	oscAmtSlider.setSliderStyle(Slider::LinearBarVertical);
	oscAmtSlider.setRange(0.0, 1.0, 0.01);
	oscAmtSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	oscAmtSlider.setPopupDisplayEnabled(true, this);
	oscAmtSlider.setTextValueSuffix(" Modulation");
	oscAmtSlider.setValue(processor.oscAmtVal);

	oscFreqSlider.setSliderStyle(Slider::LinearBarVertical);
	oscFreqSlider.setRange(0.0, 1.0, 0.01);
	oscFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	oscFreqSlider.setPopupDisplayEnabled(true, this);
	oscFreqSlider.setTextValueSuffix(" Frequency");
	oscFreqSlider.setValue(processor.oscAmtVal);

	//add the slider to the editor
	addAndMakeVisible(&drySlider);
	addAndMakeVisible(&wetSlider);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(&delaySlider);
	addAndMakeVisible(&oscAmtSlider);
	addAndMakeVisible(&oscFreqSlider);

	//add the listener to the volume slider
	drySlider.addListener(this);
	wetSlider.addListener(this);
	feedbackSlider.addListener(this);
	delaySlider.addListener(this);
	oscAmtSlider.addListener(this);
	oscFreqSlider.addListener(this);
}

//add listener function for volume slider
void Juce_vst2AudioProcessorEditor::sliderValueChanged(Slider* slider) {
	processor.dryVal = drySlider.getValue();
	processor.wetVal = wetSlider.getValue();
	processor.feedbackVal = feedbackSlider.getValue();
	processor.delayVal = delaySlider.getValue();
	processor.oscAmtVal = oscAmtSlider.getValue();
	processor.oscFreqVal = oscFreqSlider.getValue();
}

Juce_vst2AudioProcessorEditor::~Juce_vst2AudioProcessorEditor()
{
}

//==============================================================================
void Juce_vst2AudioProcessorEditor::paint (Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::white);

	g.setColour(Colours::black);
	g.setFont(15.0f);
	g.drawFittedText("Dry", 30, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Wet", 100, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Fbk", 170, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Del", 240, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Mod", 310, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Freq", 380, 0, getWidth(), 30, Justification::bottom, 1);
}

void Juce_vst2AudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	drySlider.setBounds(30, 30, 20, getHeight() - 60);
	wetSlider.setBounds(100, 30, 20, getHeight() - 60);
	feedbackSlider.setBounds(170, 30, 20, getHeight() - 60);
	delaySlider.setBounds(240, 30, 20, getHeight() - 60);
	oscAmtSlider.setBounds(310, 30, 20, getHeight() - 60);
	oscFreqSlider.setBounds(380, 30, 20, getHeight() - 60);
}
