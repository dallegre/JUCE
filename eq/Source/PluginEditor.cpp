<<<<<<< HEAD
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
    setSize (430, 300);
    
    amp1Slider.setSliderStyle(Slider::LinearBarVertical);
    amp1Slider.setRange(0.0, 1.0, 0.001);
    amp1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    amp1Slider.setPopupDisplayEnabled(true, true, this, 100);
    amp1Slider.setTextValueSuffix(" amp1");
    amp1Slider.setValue(processor.amp1Val);
    
    freq1Slider.setSliderStyle(Slider::LinearBarVertical);
    freq1Slider.setRange(0.0, 1.0, 0.001);
    freq1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    freq1Slider.setPopupDisplayEnabled(true, true, this, 100);
    freq1Slider.setTextValueSuffix(" freq1");
    freq1Slider.setValue(processor.freq1Val);

	q1Slider.setSliderStyle(Slider::LinearBarVertical);
	q1Slider.setRange(0.0, 1.0, 0.001);
	q1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	q1Slider.setPopupDisplayEnabled(true, true, this, 100);
	q1Slider.setTextValueSuffix(" q1");
	q1Slider.setValue(processor.q1Val);
    
    amp2Slider.setSliderStyle(Slider::LinearBarVertical);
    amp2Slider.setRange(0.0, 1.0, 0.001);
    amp2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    amp2Slider.setPopupDisplayEnabled(true, true, this, 100);
    amp2Slider.setTextValueSuffix(" amp2");
    amp2Slider.setValue(processor.amp2Val);
    
    freq2Slider.setSliderStyle(Slider::LinearBarVertical);
    freq2Slider.setRange(0.0, 1.0, 0.001);
    freq2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    freq2Slider.setPopupDisplayEnabled(true, true, this, 100);
    freq2Slider.setTextValueSuffix(" freq2");
    freq2Slider.setValue(processor.freq2Val);

	q2Slider.setSliderStyle(Slider::LinearBarVertical);
	q2Slider.setRange(0.0, 1.0, 0.001);
	q2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	q2Slider.setPopupDisplayEnabled(true, true, this, 100);
	q2Slider.setTextValueSuffix(" q2");
	q2Slider.setValue(processor.q2Val);

	amp3Slider.setSliderStyle(Slider::LinearBarVertical);
	amp3Slider.setRange(0.0, 1.0, 0.001);
	amp3Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	amp3Slider.setPopupDisplayEnabled(true, true, this, 100);
	amp3Slider.setTextValueSuffix(" amp3");
	amp3Slider.setValue(processor.amp3Val);

	freq3Slider.setSliderStyle(Slider::LinearBarVertical);
	freq3Slider.setRange(0.0, 1.0, 0.001);
	freq3Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	freq3Slider.setPopupDisplayEnabled(true, true, this, 100);
	freq3Slider.setTextValueSuffix(" freq3");
	freq3Slider.setValue(processor.freq3Val);

	q3Slider.setSliderStyle(Slider::LinearBarVertical);
	q3Slider.setRange(0.0, 1.0, 0.001);
	q3Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	q3Slider.setPopupDisplayEnabled(true, true, this, 100);
	q3Slider.setTextValueSuffix(" q3");
	q3Slider.setValue(processor.q3Val);

	gainSlider.setSliderStyle(Slider::LinearBarVertical);
	gainSlider.setRange(0.0, 1.0, 0.001);
	gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	gainSlider.setPopupDisplayEnabled(true, true, this, 100);
	gainSlider.setTextValueSuffix(" gain");
	gainSlider.setValue(processor.gainVal);
    
	addAndMakeVisible(&amp1Slider);
	addAndMakeVisible(&freq1Slider);
	addAndMakeVisible(&q1Slider);
	addAndMakeVisible(&amp2Slider);
	addAndMakeVisible(&freq2Slider);
	addAndMakeVisible(&q2Slider);
	addAndMakeVisible(&amp3Slider);
	addAndMakeVisible(&freq3Slider);
	addAndMakeVisible(&q3Slider);
	addAndMakeVisible(&gainSlider);
    
    amp1Slider.addListener(this);
    freq1Slider.addListener(this);
	q1Slider.addListener(this);
    amp2Slider.addListener(this);
    freq2Slider.addListener(this);
	q2Slider.addListener(this);
	amp3Slider.addListener(this);
	freq3Slider.addListener(this);
	q3Slider.addListener(this);
	gainSlider.addListener(this);

}

void EqAudioProcessorEditor::sliderValueChanged(Slider* slider){
    processor.amp1Val =  amp1Slider.getValue();
    processor.freq1Val = freq1Slider.getValue();
	processor.q1Val =    q1Slider.getValue();
    processor.amp2Val =  amp2Slider.getValue();
    processor.freq2Val = freq2Slider.getValue();
	processor.q2Val =    q2Slider.getValue();
	processor.amp3Val =  amp3Slider.getValue();
	processor.freq3Val = freq3Slider.getValue();
	processor.q3Val =    q3Slider.getValue();
	processor.gainVal =  gainSlider.getValue();
    
    processor.amp1ValScaled  = (processor.amp1Val - 0.5f);
    processor.amp2ValScaled  = (processor.amp2Val - 0.5f);
	processor.amp3ValScaled = (processor.amp3Val - 0.5f);
    processor.freq1ValScaled = 10000.0f * pow(processor.freq1Val, 3.0f);
    processor.freq2ValScaled = 10000.0f * pow(processor.freq2Val, 3.0f);
	processor.freq3ValScaled = 10000.0f * pow(processor.freq3Val, 3.0f);
    
    for(int i = 0; i < 2; i++){
        processor.filter1[i].setFc(processor.freq1ValScaled, processor.amp1ValScaled, processor.q1Val);
        processor.filter2[i].setFc(processor.freq2ValScaled, processor.amp2ValScaled, processor.q2Val);
		processor.filter3[i].setFc(processor.freq3ValScaled, processor.amp3ValScaled, processor.q3Val);
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
    
    
    g.drawFittedText("Band1", 30,  0,  getWidth(),  30, Justification::bottom, 1);
    g.drawFittedText("amp",   30,  30, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("freq",  60,  30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("q",     90,  30, getWidth(), 30, Justification::bottom, 1);
    
    g.drawFittedText("Band2", 150, 0,  getWidth(),  30, Justification::bottom, 1);
    g.drawFittedText("amp",   150, 30, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("freq",  180, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("q",     210, 30, getWidth(), 30, Justification::bottom, 1);

	g.drawFittedText("Band3", 270, 0,  getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("amp",   270, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("freq",  300, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("q",     330, 30, getWidth(), 30, Justification::bottom, 1);

	g.drawFittedText("amp",   390, 30, getWidth(), 30, Justification::bottom, 1);
}

void EqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    amp1Slider.setBounds    (30, 60, 20, getHeight() - 90);
    freq1Slider.setBounds   (60, 60, 20, getHeight() - 90);
	q1Slider.setBounds      (90, 60, 20,  getHeight() - 90);
    
    amp2Slider.setBounds    (150, 60, 20, getHeight() - 90);
    freq2Slider.setBounds   (180, 60, 20, getHeight() - 90);
	q2Slider.setBounds      (210, 60, 20, getHeight() - 90);

	amp3Slider.setBounds    (270, 60, 20, getHeight() - 90);
	freq3Slider.setBounds   (300, 60, 20, getHeight() - 90);
	q3Slider.setBounds      (330, 60, 20, getHeight() - 90);

	gainSlider.setBounds    (390, 60, 20, getHeight() - 90);
    
}
=======
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
    /*
    setSize (430, 300);
    
    amp1Slider.setSliderStyle(Slider::LinearBarVertical);
    amp1Slider.setRange(0.0, 1.0, 0.001);
    amp1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    //amp1Slider.setPopupDisplayEnabled(true, this);
    amp1Slider.setTextValueSuffix(" amp1");
    //amp1Slider.setValue(processor.amp1Val);
    
    freq1Slider.setSliderStyle(Slider::LinearBarVertical);
    freq1Slider.setRange(0.0, 1.0, 0.001);
    freq1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    //freq1Slider.setPopupDisplayEnabled(true, this);
    freq1Slider.setTextValueSuffix(" freq1");
    //freq1Slider.setValue(processor.freq1Val);

	q1Slider.setSliderStyle(Slider::LinearBarVertical);
	q1Slider.setRange(0.0, 1.0, 0.001);
	q1Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//q1Slider.setPopupDisplayEnabled(true, this);
	q1Slider.setTextValueSuffix(" q1");
	//q1Slider.setValue(processor.q1Val);
    
    amp2Slider.setSliderStyle(Slider::LinearBarVertical);
    amp2Slider.setRange(0.0, 1.0, 0.001);
    amp2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    //amp2Slider.setPopupDisplayEnabled(true, this);
    amp2Slider.setTextValueSuffix(" amp2");
    //amp2Slider.setValue(processor.amp2Val);
    
    freq2Slider.setSliderStyle(Slider::LinearBarVertical);
    freq2Slider.setRange(0.0, 1.0, 0.001);
    freq2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    //freq2Slider.setPopupDisplayEnabled(true, this);
    freq2Slider.setTextValueSuffix(" freq2");
    //freq2Slider.setValue(processor.freq2Val);

	q2Slider.setSliderStyle(Slider::LinearBarVertical);
	q2Slider.setRange(0.0, 1.0, 0.001);
	q2Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//q2Slider.setPopupDisplayEnabled(true, this);
	q2Slider.setTextValueSuffix(" q2");
	//q2Slider.setValue(processor.q2Val);

	amp3Slider.setSliderStyle(Slider::LinearBarVertical);
	amp3Slider.setRange(0.0, 1.0, 0.001);
	amp3Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//amp3Slider.setPopupDisplayEnabled(true, this);
	amp3Slider.setTextValueSuffix(" amp3");
	//amp3Slider.setValue(processor.amp3Val);

	freq3Slider.setSliderStyle(Slider::LinearBarVertical);
	freq3Slider.setRange(0.0, 1.0, 0.001);
	freq3Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//freq3Slider.setPopupDisplayEnabled(true, this);
	freq3Slider.setTextValueSuffix(" freq3");
	//freq3Slider.setValue(processor.freq3Val);

	q3Slider.setSliderStyle(Slider::LinearBarVertical);
	q3Slider.setRange(0.0, 1.0, 0.001);
	q3Slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//q3Slider.setPopupDisplayEnabled(true, this);
	q3Slider.setTextValueSuffix(" q3");
	//q3Slider.setValue(processor.q3Val);

	gainSlider.setSliderStyle(Slider::LinearBarVertical);
	gainSlider.setRange(0.0, 1.0, 0.001);
	gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	//gainSlider.setPopupDisplayEnabled(true, this);
	gainSlider.setTextValueSuffix(" gain");
	//gainSlider.setValue(processor.gainVal);
    
	addAndMakeVisible(&amp1Slider);
	addAndMakeVisible(&freq1Slider);
	addAndMakeVisible(&q1Slider);
	addAndMakeVisible(&amp2Slider);
	addAndMakeVisible(&freq2Slider);
	addAndMakeVisible(&q2Slider);
	addAndMakeVisible(&amp3Slider);
	addAndMakeVisible(&freq3Slider);
	addAndMakeVisible(&q3Slider);
	addAndMakeVisible(&gainSlider);
    
    amp1Slider.addListener(this);
    freq1Slider.addListener(this);
	q1Slider.addListener(this);
    amp2Slider.addListener(this);
    freq2Slider.addListener(this);
	q2Slider.addListener(this);
	amp3Slider.addListener(this);
	freq3Slider.addListener(this);
	q3Slider.addListener(this);
	gainSlider.addListener(this);
*/
}

void EqAudioProcessorEditor::sliderValueChanged(Slider* slider){
    /*
    processor.amp1Val =  amp1Slider.getValue();
    processor.freq1Val = freq1Slider.getValue();
	processor.q1Val =    q1Slider.getValue();
    processor.amp2Val =  amp2Slider.getValue();
    processor.freq2Val = freq2Slider.getValue();
	processor.q2Val =    q2Slider.getValue();
	processor.amp3Val =  amp3Slider.getValue();
	processor.freq3Val = freq3Slider.getValue();
	processor.q3Val =    q3Slider.getValue();
	processor.gainVal =  gainSlider.getValue();
    
    processor.amp1ValScaled  = (processor.amp1Val - 0.5f);
    processor.amp2ValScaled  = (processor.amp2Val - 0.5f);
	processor.amp3ValScaled = (processor.amp3Val - 0.5f);
    processor.freq1ValScaled = 10000.0f * pow(processor.freq1Val, 3.0f);
    processor.freq2ValScaled = 10000.0f * pow(processor.freq2Val, 3.0f);
	processor.freq3ValScaled = 10000.0f * pow(processor.freq3Val, 3.0f);
    
    for(int i = 0; i < 2; i++){
        processor.filter1[i].setFc(processor.freq1ValScaled, processor.amp1ValScaled, processor.q1Val);
        processor.filter2[i].setFc(processor.freq2ValScaled, processor.amp2ValScaled, processor.q2Val);
		processor.filter3[i].setFc(processor.freq3ValScaled, processor.amp3ValScaled, processor.q3Val);
    }
     */
}

EqAudioProcessorEditor::~EqAudioProcessorEditor()
{
}

//==============================================================================
void EqAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
  /*
    g.fillAll(Colours::white);
    
    g.setColour(Colours::black);
    g.setFont(15.0f);
    
    
    g.drawFittedText("Band1", 30,  0,  getWidth(),  30, Justification::bottom, 1);
    g.drawFittedText("amp",   30,  30, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("freq",  60,  30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("q",     90,  30, getWidth(), 30, Justification::bottom, 1);
    
    g.drawFittedText("Band2", 150, 0,  getWidth(),  30, Justification::bottom, 1);
    g.drawFittedText("amp",   150, 30, getWidth(), 30, Justification::bottom, 1);
    g.drawFittedText("freq",  180, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("q",     210, 30, getWidth(), 30, Justification::bottom, 1);

	g.drawFittedText("Band3", 270, 0,  getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("amp",   270, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("freq",  300, 30, getWidth(), 30, Justification::bottom, 1);
	g.drawFittedText("q",     330, 30, getWidth(), 30, Justification::bottom, 1);

	g.drawFittedText("amp",   390, 30, getWidth(), 30, Justification::bottom, 1);
   */
}

void EqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    /*
    amp1Slider.setBounds    (30, 60, 20, getHeight() - 90);
    freq1Slider.setBounds   (60, 60, 20, getHeight() - 90);
	q1Slider.setBounds      (90, 60, 20,  getHeight() - 90);
    
    amp2Slider.setBounds    (150, 60, 20, getHeight() - 90);
    freq2Slider.setBounds   (180, 60, 20, getHeight() - 90);
	q2Slider.setBounds      (210, 60, 20, getHeight() - 90);

	amp3Slider.setBounds    (270, 60, 20, getHeight() - 90);
	freq3Slider.setBounds   (300, 60, 20, getHeight() - 90);
	q3Slider.setBounds      (330, 60, 20, getHeight() - 90);

	gainSlider.setBounds    (390, 60, 20, getHeight() - 90);
    */
}
>>>>>>> 0f0dd64d30e09ee0b62f50efd939db6e97d02703
