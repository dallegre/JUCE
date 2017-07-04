/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Juce_vstAudioProcessorEditor::Juce_vstAudioProcessorEditor (Juce_vstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(400, 300);
	setResizable(false, true);

	//the constructor gets called every time you click on the plugin so initialize everything at prepare to play
	//set up parameters for the slider
	volumeSlider.setSliderStyle(Slider::LinearBarVertical);
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	volumeSlider.setPopupDisplayEnabled(true, this);
	volumeSlider.setTextValueSuffix(" Volume");
	volumeSlider.setValue(processor.volumeVal);

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

	//add the slider to the editor
	addAndMakeVisible(&volumeSlider);
	addAndMakeVisible(&wetSlider);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(&delaySlider);

	//add the listener to the volume slider
	volumeSlider.addListener(this);
	wetSlider.addListener(this);
	feedbackSlider.addListener(this);
	delaySlider.addListener(this);
}

//add listener function for volume slider
void Juce_vstAudioProcessorEditor::sliderValueChanged(Slider* slider) {
	processor.volumeVal = volumeSlider.getValue();
	processor.wetVal = wetSlider.getValue();
	processor.feedbackVal = feedbackSlider.getValue();
	processor.delayVal = delaySlider.getValue();
}

Juce_vstAudioProcessorEditor::~Juce_vstAudioProcessorEditor()
{
}

//==============================================================================
void Juce_vstAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::white);

	g.setColour(Colours::black);
	g.setFont(15.0f);
	g.drawFittedText("Volume", 0, 0, getWidth(), 30, Justification::left, 1);
	g.drawFittedText("Wet", 75, 0, getWidth(), 30, Justification::left, 1);
	g.drawFittedText("Feedback", 150, 0, getWidth(), 30, Justification::left, 1);
	g.drawFittedText("Delay", 225, 0, getWidth(), 30, Justification::left, 1);
}

void Juce_vstAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	volumeSlider.setBounds(30, 30, 20, getHeight() - 60);
	wetSlider.setBounds(100, 30, 20, getHeight() - 60);
	feedbackSlider.setBounds(170, 30, 20, getHeight() - 60);
	delaySlider.setBounds(240, 30, 20, getHeight() - 60);
}
