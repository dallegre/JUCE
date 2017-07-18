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
SynthAudioProcessorEditor::SynthAudioProcessorEditor (SynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (230, 300);
    
    oscSlider.setSliderStyle(Slider::LinearBarVertical);
    oscSlider.setRange(0.0, 1.0, 0.01);
    oscSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    oscSlider.setPopupDisplayEnabled(true, this);
    oscSlider.setTextValueSuffix(" freq");
    oscSlider.setValue(processor.oscVal);
    
    detSlider.setSliderStyle(Slider::LinearBarVertical);
    detSlider.setRange(0.0, 1.0, 0.01);
    detSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    detSlider.setPopupDisplayEnabled(true, this);
    detSlider.setTextValueSuffix(" detune");
    detSlider.setValue(processor.detVal);
    
    freqSlider.setSliderStyle(Slider::LinearBarVertical);
    freqSlider.setRange(0.0, 1.0, 0.01);
    freqSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    freqSlider.setPopupDisplayEnabled(true, this);
    freqSlider.setTextValueSuffix(" freq");
    freqSlider.setValue(processor.freqVal);

    qSlider.setSliderStyle(Slider::LinearBarVertical);
    qSlider.setRange(0.0, 1.0, 0.01);
    qSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    qSlider.setPopupDisplayEnabled(true, this);
    qSlider.setTextValueSuffix(" q");
    qSlider.setValue(processor.qVal);
    
    envSlider.setSliderStyle(Slider::LinearBarVertical);
    envSlider.setRange(0.0, 1.0, 0.01);
    envSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    envSlider.setPopupDisplayEnabled(true, this);
    envSlider.setTextValueSuffix(" env");
    envSlider.setValue(processor.envVal);
    
    speedSlider.setSliderStyle(Slider::LinearBarVertical);
    speedSlider.setRange(0.0, 1.0, 0.01);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    speedSlider.setPopupDisplayEnabled(true, this);
    speedSlider.setTextValueSuffix(" env");
    speedSlider.setValue(processor.speedVal);
    
    addAndMakeVisible(&oscSlider);
    addAndMakeVisible(&detSlider);
    addAndMakeVisible(&freqSlider);
    addAndMakeVisible(&qSlider);
    addAndMakeVisible(&envSlider);
    addAndMakeVisible(&speedSlider);
    
    oscSlider.addListener(this);
    detSlider.addListener(this);
    freqSlider.addListener(this);
    qSlider.addListener(this);
    envSlider.addListener(this);
    speedSlider.addListener(this);
    
}

void SynthAudioProcessorEditor::sliderValueChanged(Slider* slider){
    
    processor.oscVal =   oscSlider.getValue();
    processor.detVal =   detSlider.getValue();
    processor.freqVal =  freqSlider.getValue();
    processor.qVal =     qSlider.getValue();
    processor.envVal =   envSlider.getValue();
    processor.speedVal = speedSlider.getValue();
    
    processor.freqValScaled = 20000.0f * pow(processor.freqVal, 3.0f);
    processor.envValScaled =  10000.0f *  pow(processor.envVal, 3.0f);
    processor.speedValScaled = (1.0f -  processor.speedVal);
    processor.oscValScaled =   (processor.oscVal - 0.5f) * 70.0f;
    processor.detValScaled =   (processor.detVal - 0.5f) * 7.0f;

}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::white);

    g.setColour (Colours::darkgrey);
    g.setFont (15.0f);

    g.drawFittedText("Osc",  30,  20, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("Det",  60,  20, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("Freq", 90,  20, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("Res",  120, 20, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("Env",  150, 20, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("Rate", 180, 20, getWidth(), 30, Justification::bottom, 1);
} 

void SynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    oscSlider.setBounds(  30,  50, 20, getHeight() - 90);
    detSlider.setBounds(  60,  50, 20, getHeight() - 90);
    freqSlider.setBounds( 90,  50, 20, getHeight() - 90);
    qSlider.setBounds(    120, 50, 20, getHeight() - 90);
    envSlider.setBounds(  150, 50, 20, getHeight() - 90);
    speedSlider.setBounds(180, 50, 20, getHeight() - 90);
}
