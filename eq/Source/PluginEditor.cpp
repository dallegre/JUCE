/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
EqAudioProcessorEditor::EqAudioProcessorEditor (EqAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    amp1Slider.setSliderStyle(Slider::LinearBarVertical);
    amp1Slider.setRange(0.0, 1.0, 0.001);
    amp1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    amp1Slider.setPopupDisplayEnabled(true, this);
    amp1Slider.setTextValueSuffix(" amp1");
    amp1Slider.setValue(processor.amp1Val);
    
    freq1Slider.setSliderStyle(Slider::LinearBarVertical);
    freq1Slider.setRange(0.0, 1.0, 0.001);
    freq1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    freq1Slider.setPopupDisplayEnabled(true, this);
    freq1Slider.setTextValueSuffix(" freq1");
    freq1Slider.setValue(processor.freq1Val);
    
    amp2Slider.setSliderStyle(Slider::LinearBarVertical);
    amp2Slider.setRange(0.0, 1.0, 0.001);
    amp2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    amp2Slider.setPopupDisplayEnabled(true, this);
    amp2Slider.setTextValueSuffix(" amp2");
    amp2Slider.setValue(processor.amp2Val);
    
    freq2Slider.setSliderStyle(Slider::LinearBarVertical);
    freq2Slider.setRange(0.0, 1.0, 0.001);
    freq2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    freq2Slider.setPopupDisplayEnabled(true, this);
    freq2Slider.setTextValueSuffix(" freq2");
    freq2Slider.setValue(processor.freq2Val);
    
	addAndMakeVisible(&amp1Slider);
	addAndMakeVisible(&freq1Slider);
	addAndMakeVisible(&amp2Slider);
	addAndMakeVisible(&freq2Slider);
    
    amp1Slider.addListener(this);
    freq1Slider.addListener(this);
    amp2Slider.addListener(this);
    freq2Slider.addListener(this);

}

void EqAudioProcessorEditor::sliderValueChanged(Slider* slider){
    processor.amp1Val =  amp1Slider.getValue();
    processor.freq1Val = freq1Slider.getValue();
    processor.amp2Val =  amp2Slider.getValue();
    processor.freq2Val = freq2Slider.getValue();
    
    processor.amp1ValScaled  = (processor.amp1Val - 0.5f);
    processor.amp2ValScaled  = (processor.amp2Val - 0.5f);
    processor.freq1ValScaled = 5000.0f * processor.freq1Val;
    processor.freq2ValScaled = 5000.0f * processor.freq2Val;
    
    for(int i = 0; i < 2; i++){
        processor.filter1[i].setFc(processor.freq1ValScaled, processor.amp1ValScaled, 2.0f);
        processor.filter2[i].setFc(processor.freq2ValScaled, processor.amp2ValScaled, 2.0f);
    }
}

EqAudioProcessorEditor::~EqAudioProcessorEditor()
{
}

//==============================================================================
void EqAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white);
    
    g.setColour(Colours::black);
    g.setFont(15.0f);
    
    
    g.drawFittedText("Band1", 30,  0, getWidth(),  30, Justification::bottom, 1);
    g.drawFittedText("amp",   30,  30, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("freq",  60,  30, getWidth(), 30, Justification::bottom, 1);
    
    g.drawFittedText("Band2", 120, 0, getWidth(),  30, Justification::bottom, 1);
    g.drawFittedText("amp",   120, 30, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("freq",  150, 30, getWidth(), 30, Justification::bottom, 1);
}

void EqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    amp1Slider.setBounds    (30,  60, 20, getHeight() - 90);
    freq1Slider.setBounds   (60,  60, 20, getHeight() - 90);
    
    amp2Slider.setBounds    (120, 60, 20, getHeight() - 90);
    freq2Slider.setBounds   (150, 60, 20, getHeight() - 90);
    
}
