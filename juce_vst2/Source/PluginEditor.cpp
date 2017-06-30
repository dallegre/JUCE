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
    setSize (400, 300);
	setResizable(false, true);

	//the constructor gets called every time you click on the plugin so initialize everything at prepare to play
	//set up parameters for the slider
	volumeSlider.setSliderStyle(Slider::LinearBarVertical);
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	volumeSlider.setPopupDisplayEnabled(true, this);
	volumeSlider.setTextValueSuffix(" Volume");
	volumeSlider.setValue(processor.volumeVal);
	//add the slider to the editor
	addAndMakeVisible(&volumeSlider);
	//add the listener to the volume slider
	volumeSlider.addListener(this);
}

//add listener function for volume slider
void Juce_vst2AudioProcessorEditor::sliderValueChanged(Slider* slider) {
	processor.volumeVal = volumeSlider.getValue();
}

Juce_vst2AudioProcessorEditor::~Juce_vst2AudioProcessorEditor()
{
}

//==============================================================================
void Juce_vst2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Volume", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void Juce_vst2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	//dynamically set the position of the slider
	volumeSlider.setBounds(40, 30, 20, getHeight() - 60);
}
