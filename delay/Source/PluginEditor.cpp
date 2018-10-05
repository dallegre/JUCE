/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

//==============================================================================
Juce_vst2AudioProcessorEditor::Juce_vst2AudioProcessorEditor (Juce_vst2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);
	setResizable(false, true);

	//the constructor gets called every time you click on the plugin so initialize everything at prepare to play
	//set up parameters for the slider
	filterFreqSlider.setSliderStyle(Slider::LinearBarVertical);
	filterFreqSlider.setRange(0.0, 1.0, 0.001);
	filterFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//filterFreqSlider.setPopupDisplayEnabled(true, this);
	filterFreqSlider.setTextValueSuffix(" Cutoff");
	filterFreqSlider.setValue(processor.filterFreqVal);

	filterQSlider.setSliderStyle(Slider::LinearBarVertical);
	filterQSlider.setRange(0.0, 1.0, 0.01);
	filterQSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//filterQSlider.setPopupDisplayEnabled(true, this);
	filterQSlider.setTextValueSuffix(" Cutoff");
	filterQSlider.setValue(processor.filterQVal);



	filter2FreqSlider.setSliderStyle(Slider::LinearBarVertical);
	filter2FreqSlider.setRange(0.0, 1.0, 0.001);
	filter2FreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//filter2FreqSlider.setPopupDisplayEnabled(true, this);
	filter2FreqSlider.setTextValueSuffix(" Cutoff");
	filter2FreqSlider.setValue(processor.filter2FreqVal);

	filter2QSlider.setSliderStyle(Slider::LinearBarVertical);
	filter2QSlider.setRange(0.0, 1.0, 0.01);
	filter2QSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//filter2QSlider.setPopupDisplayEnabled(true, this);
	filter2QSlider.setTextValueSuffix(" Cutoff");
	filter2QSlider.setValue(processor.filter2QVal);




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
	delaySlider.setRange(0.0, 1.0, 0.001);
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
	addAndMakeVisible(&filterFreqSlider);
	addAndMakeVisible(&filterQSlider);

	addAndMakeVisible(&filter2FreqSlider);
	addAndMakeVisible(&filter2QSlider);

	addAndMakeVisible(&drySlider);
	addAndMakeVisible(&wetSlider);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(&delaySlider);
	addAndMakeVisible(&oscAmtSlider);
	addAndMakeVisible(&oscFreqSlider);

	//add the listener to the volume slider
	filterFreqSlider.addListener(this);
	filterQSlider.addListener(this);

	filter2FreqSlider.addListener(this);
	filter2QSlider.addListener(this);

	drySlider.addListener(this);
	wetSlider.addListener(this);
	feedbackSlider.addListener(this);
	delaySlider.addListener(this);
	oscAmtSlider.addListener(this);
	oscFreqSlider.addListener(this);

}

//add listener function for volume slider
void Juce_vst2AudioProcessorEditor::sliderValueChanged(Slider* slider) {

	processor.filterFreqVal =  filterFreqSlider.getValue();
	processor.filterQVal =     filterQSlider.getValue();

	processor.filter2FreqVal = filter2FreqSlider.getValue();
	processor.filter2QVal =    filter2QSlider.getValue();

	processor.dryVal =         drySlider.getValue();
	processor.wetVal =         wetSlider.getValue();
	processor.feedbackVal =    feedbackSlider.getValue();
	processor.delayVal =	   delaySlider.getValue();
	processor.oscAmtVal =      oscAmtSlider.getValue();
	processor.oscFreqVal =     oscFreqSlider.getValue();
    
    processor.filterFreqScaled =  10000.0f *  pow(processor.filterFreqVal, 3.0);
    processor.filter2FreqScaled = 20000.0f *  pow(processor.filter2FreqVal, 3.0);
    
    processor.oscAmtValScaled =  50.0f *   processor.oscAmtVal;
    processor.oscFreqValScaled = 2.0f *    processor.oscFreqVal;

    for(int i = 0; i < 2; i++){
        processor.svfilter[i].setFc(processor.filterFreqScaled, UPSAMPLING);
        processor.svfilter[i].setQ(processor.filterQVal);
        processor.svfilter2[i].setFc(processor.filter2FreqScaled, UPSAMPLING);
        processor.svfilter2[i].setQ(processor.filter2QVal);
    }

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

	g.drawFittedText("Highpass", 30, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Cut", 30, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Q",   60, 30, getWidth(), 30, Justification::bottom, 1);

	g.drawFittedText("Lowpass", 120, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Cut", 120, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Q",   150, 30, getWidth(), 30, Justification::bottom, 1);
	
	g.drawFittedText("Delay", 210, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Dry", 210, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Wet", 240, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Fbk", 270, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Del", 300, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Mod", 330, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Freq",360, 30, getWidth(), 30, Justification::bottom, 1);
}

void Juce_vst2AudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	filterFreqSlider.setBounds (30, 60, 20, getHeight() - 90);
	filterQSlider.setBounds    (60, 60, 20, getHeight() - 90);

	filter2FreqSlider.setBounds(120, 60, 20, getHeight() - 90);
	filter2QSlider.setBounds   (150, 60, 20, getHeight() - 90);

	drySlider.setBounds        (210, 60, 20, getHeight() - 90);
	wetSlider.setBounds        (240, 60, 20, getHeight() - 90);
	feedbackSlider.setBounds   (270, 60, 20, getHeight() - 90);
	delaySlider.setBounds      (300, 60, 20, getHeight() - 90);
	oscAmtSlider.setBounds     (330, 60, 20, getHeight() - 90);
	oscFreqSlider.setBounds    (360, 60, 20, getHeight() - 90);

}
