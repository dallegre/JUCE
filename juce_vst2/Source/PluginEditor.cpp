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
	setSize(620, 300);
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


	filterFreqSlider.setSliderStyle(Slider::LinearBarVertical);
	filterFreqSlider.setRange(0.0, 1.0, 0.01);
	filterFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filterFreqSlider.setPopupDisplayEnabled(true, this);
	filterFreqSlider.setTextValueSuffix(" Cutoff");
	filterFreqSlider.setValue(processor.filterFreqVal);

	filterQSlider.setSliderStyle(Slider::LinearBarVertical);
	filterQSlider.setRange(0.0, 1.0, 0.01);
	filterQSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filterQSlider.setPopupDisplayEnabled(true, this);
	filterQSlider.setTextValueSuffix(" Cutoff");
	filterQSlider.setValue(processor.filterQVal);

	filterAmpSlider.setSliderStyle(Slider::LinearBarVertical);
	filterAmpSlider.setRange(0.0, 1.0, 0.01);
	filterAmpSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filterAmpSlider.setPopupDisplayEnabled(true, this);
	filterAmpSlider.setTextValueSuffix(" Cutoff");
	filterAmpSlider.setValue(processor.filterAmpVal);



	filter2FreqSlider.setSliderStyle(Slider::LinearBarVertical);
	filter2FreqSlider.setRange(0.0, 1.0, 0.01);
	filter2FreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filter2FreqSlider.setPopupDisplayEnabled(true, this);
	filter2FreqSlider.setTextValueSuffix(" Cutoff");
	filter2FreqSlider.setValue(processor.filter2FreqVal);

	filter2QSlider.setSliderStyle(Slider::LinearBarVertical);
	filter2QSlider.setRange(0.0, 1.0, 0.01);
	filter2QSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filter2QSlider.setPopupDisplayEnabled(true, this);
	filter2QSlider.setTextValueSuffix(" Cutoff");
	filter2QSlider.setValue(processor.filter2QVal);

	filter2AmpSlider.setSliderStyle(Slider::LinearBarVertical);
	filter2AmpSlider.setRange(0.0, 1.0, 0.01);
	filter2AmpSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filter2AmpSlider.setPopupDisplayEnabled(true, this);
	filter2AmpSlider.setTextValueSuffix(" Cutoff");
	filter2AmpSlider.setValue(processor.filter2AmpVal);

	
	//add the slider to the editor
	addAndMakeVisible(&drySlider);
	addAndMakeVisible(&wetSlider);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(&delaySlider);
	addAndMakeVisible(&oscAmtSlider);
	addAndMakeVisible(&oscFreqSlider);

	addAndMakeVisible(&filterFreqSlider);
	addAndMakeVisible(&filterQSlider);
	addAndMakeVisible(&filterAmpSlider);
	
	addAndMakeVisible(&filter2FreqSlider);
	addAndMakeVisible(&filter2QSlider);
	addAndMakeVisible(&filter2AmpSlider);

	//add the listener to the volume slider
	drySlider.addListener(this);
	wetSlider.addListener(this);
	feedbackSlider.addListener(this);
	delaySlider.addListener(this);
	oscAmtSlider.addListener(this);
	oscFreqSlider.addListener(this);

	filterFreqSlider.addListener(this);
	filterQSlider.addListener(this);
	filterAmpSlider.addListener(this);
	
	filter2FreqSlider.addListener(this);
	filter2QSlider.addListener(this);
	filter2AmpSlider.addListener(this);

}

//add listener function for volume slider
void Juce_vst2AudioProcessorEditor::sliderValueChanged(Slider* slider) {

	processor.dryVal =         drySlider.getValue();
	processor.wetVal =         wetSlider.getValue();
	processor.feedbackVal =    feedbackSlider.getValue();
	processor.delayVal =       delaySlider.getValue();
	processor.oscAmtVal =      oscAmtSlider.getValue();
	processor.oscFreqVal =     oscFreqSlider.getValue();

	processor.filterFreqVal =  filterFreqSlider.getValue();
	processor.filterQVal =     filterQSlider.getValue();
	processor.filterAmpVal =   filterAmpSlider.getValue();

	processor.filter2FreqVal = filter2FreqSlider.getValue();
	processor.filter2QVal =    filter2QSlider.getValue();
	processor.filter2AmpVal =  filter2AmpSlider.getValue();

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
	
	g.drawFittedText("Dry", 30,  0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Wet", 60,  0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Fbk", 90,  0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Del", 120, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Mod", 150, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Freq",180, 0, getWidth(), 30, Justification::bottom, 1);
	
	g.drawFittedText("Cut", 250, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Q",   280, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Amp", 310, 0, getWidth(), 30, Justification::bottom, 1);
	
	g.drawFittedText("Cut", 380, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Q",   410, 0, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("Amp", 440, 0, getWidth(), 30, Justification::bottom, 1);
}

void Juce_vst2AudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	drySlider.setBounds     (30,  30, 20, getHeight() - 60);
	wetSlider.setBounds     (60,  30, 20, getHeight() - 60);
	feedbackSlider.setBounds(90,  30, 20, getHeight() - 60);
	delaySlider.setBounds   (120, 30, 20, getHeight() - 60);
	oscAmtSlider.setBounds  (150, 30, 20, getHeight() - 60);
	oscFreqSlider.setBounds (180, 30, 20, getHeight() - 60);
	
	filterFreqSlider.setBounds(250, 30, 20, getHeight() - 60);
	filterQSlider.setBounds   (280, 30, 20, getHeight() - 60);
	filterAmpSlider.setBounds (310, 30, 20, getHeight() - 60);
	
	filter2FreqSlider.setBounds(380, 30, 20, getHeight() - 60);
	filter2QSlider.setBounds   (410, 30, 20, getHeight() - 60);
	filter2AmpSlider.setBounds (440, 30, 20, getHeight() - 60);

}
