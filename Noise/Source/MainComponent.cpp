/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>
#include "../Builds/VisualStudio2015/lowpass.h"
#include "../Builds/VisualStudio2015/highpass.h"
#include "../Builds/VisualStudio2015/osc.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public AudioAppComponent,
	Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
		//sets the size of the window
        setSize(400, 200);
		volumeSlider.addListener(this);
		freqSlider.addListener(this);
		fbSlider.addListener(this);
		modaSlider.addListener(this);
		modfSlider.addListener(this);

		addAndMakeVisible(&volumeSlider);
		addAndMakeVisible(&fbSlider);
		addAndMakeVisible(&freqSlider);
		addAndMakeVisible(&modaSlider);
		addAndMakeVisible(modfSlider);

		addAndMakeVisible(&volumeLabel);
		addAndMakeVisible(&fbLabel);
		addAndMakeVisible(&freqLabel);
		addAndMakeVisible(&modaLabel);
		addAndMakeVisible(modfLabel);

		volumeSlider.setSliderStyle(Slider::LinearBar);
		freqSlider.setSliderStyle(Slider::LinearBar);
		fbSlider.setSliderStyle(Slider::LinearBar);
		modaSlider.setSliderStyle(Slider::LinearBar);
		modfSlider.setSliderStyle(Slider::LinearBar);

		volumeLabel.setText("Noise Volume",dontSendNotification);
		freqLabel.setText("Filter Cutoff", dontSendNotification);
		fbLabel.setText("Filter Q", dontSendNotification);
		modaLabel.setText("Oscillator Amplitude", dontSendNotification);
		modfLabel.setText("Oscillator Frequency", dontSendNotification);

		volumeLabel.setJustificationType(Justification::centred);
		freqLabel.setJustificationType(Justification::centred);
		fbLabel.setJustificationType(Justification::centred);
		modaLabel.setJustificationType(Justification::centred);
		modfLabel.setJustificationType(Justification::centred);
		
		volumeSlider.setRange(0.0f, 1.0f);
		fbSlider.setRange(0.0f, 1.0f);
		freqSlider.setRange(20.0f, 20000.0f);
		modaSlider.setRange(0.0f, 2000.0f);				//dc to 5khz modulation of filter
		modfSlider.setRange(0.1f, 50.0f);				//mod frequency range
		
		volumeSlider.setValue(0.2f);
		fbSlider.setValue(0.0f);
		freqSlider.setValue(10000.0f);
		modaSlider.setValue(100.0f);
		modfSlider.setValue(0.5f);

		volumeSlider.setSkewFactorFromMidPoint(0.2f);			//these set sort of an exponential characteristic
		fbSlider.setSkewFactorFromMidPoint(0.7f);				//just linear
		freqSlider.setSkewFactorFromMidPoint(2000);
		modaSlider.setSkewFactorFromMidPoint(100.0f);
		modfSlider.setSkewFactorFromMidPoint(5.0f);

		volumeLabel.setBoundsRelative(.1f, .0f, .8f, .1f);
		volumeSlider.setBoundsRelative(.1f, .1f, .8f, .1f);
		freqLabel.setBoundsRelative(.1f, .2f, .8f, .1f);
		freqSlider.setBoundsRelative(.1f, .3f, .8f, .1f);
		fbLabel.setBoundsRelative(.1f, .4f, .8f, .1f);
		fbSlider.setBoundsRelative(.1f, .5f, .8f, .1f);
		modaLabel.setBoundsRelative(.1f, .6f, .8f, .1f);
		modaSlider.setBoundsRelative(.1f, .7f, .8f, .1f);
		modfLabel.setBoundsRelative(.1f, .8f, .8f, .1f);
		modfSlider.setBoundsRelative(.1f, .9f, .8f, .1f);
        
		//2 outputs, no inputs
        setAudioChannels (0, 2);

		//shouldn't you initialize variables here?
		volumeSliderValue = 0.0; freqSliderValue = 0.0; filterOut = 0.0; filterFeedback = 0.0;
		fbSliderValue = 0.0; filterOut2 = 0.0; modfSliderValue = 0.0; modaSliderValue = 0.0;
		oscValue = 0.0f;
		sampleRateInt; initialShitHack = 0; initialShitHack2 = 0;
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

	void MainContentComponent::sliderValueChanged(Slider* slider) {
		if (&volumeSlider == slider) {
			volumeSliderValue = float(volumeSlider.getValue());
		}
		if (&freqSlider == slider){
			freqSliderValue = float(freqSlider.getValue());
		}
		if (&fbSlider == slider) {
			fbSliderValue = float(fbSlider.getValue());
		}
		if (&modaSlider == slider) {
			modaSliderValue = float(modaSlider.getValue());
		}
		if (&modfSlider == slider) {
			modfSliderValue = float(modfSlider.getValue());
			modOsc.setF(modfSliderValue / sampleRateInt, modaSliderValue);
		}
	}

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
		//initialize filters.  If these are not initialized and only set in sliverValueChanged, it will not
		//work most of the time in windows 10 as opposed to 7 where it would always work..
		volumeSliderValue = float(volumeSlider.getValue());
		freqSliderValue = float(freqSlider.getValue());
		fbSliderValue = float(fbSlider.getValue());
		modaSliderValue = float(modaSlider.getValue());
		modfSliderValue = float(modfSlider.getValue());
		modOsc.setF(modfSliderValue / sampleRateInt, modaSliderValue);
		lowpassFilter.setFc(freqSliderValue / sampleRateInt);
		lowpassFilter2.setFc(freqSliderValue / sampleRateInt);
		lowpassFilter3.setFc(freqSliderValue / sampleRateInt);
		lowpassFilter4.setFc(freqSliderValue / sampleRateInt);
		highpassFilter.setFc(30.0f / sampleRateInt);				//dc blocking in the feedback.  not sure if it's needed.
		//this isn't super necessary, but what the heck
		String message;
		message << "Preparing to play audio..." << newLine;
		message << " samplesPerBlockExpected = " << samplesPerBlockExpected << newLine;
		message << " sampleRate = " << sampleRate;
		Logger::getCurrentLogger()->writeToLog(message);
		sampleRateInt = int(sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
		//This is kind of the main dsp section..
		//AudioSourceChannelInfo contains..
		//- an AudioSampleBuffer, which is a multidimensional float array
		//this contains any audio from the inputs and where to put the data for the outputs
		//- startSample, and int that specifies the sample within the buffer that we're at
		//- numSamples, an int that specifies the number of samples available to be read and 
		//that should be written.
		//Note that these floats are scaled from 1.0 to -1.0
		
		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
			//get a pointer to the start sample in the buffer for this audio output channel
			float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {
				if (0 == channel) {
					
					//create random noise signal and apply first filter pole
					filterOut = lowpassFilter.process(satSubtract(((random.nextFloat() * 0.9f - 0.45f)*volumeSliderValue),
					filterFeedback*fbSliderValue*5.0f));
					//apply other 3 poles and create feedback signal.
					filterOut2 = lowpassFilter2.process(filterOut);
					filterOut2 = lowpassFilter3.process(filterOut2);
					filterFeedback = lowpassFilter4.process(filterOut2);
					//audio out tap
					buffer[sample] = filterFeedback;
					filterFeedback = highpassFilter.process(filterFeedback);

					//do mod oscillators
					oscValue = modOsc.process(1)*modaSliderValue;
					//limit oscillator values to ones that will be a reasonable cutoff

					if ((freqSliderValue + oscValue) < 0) {
						oscValue = -freqSliderValue*0.9f;
					}
					if ((freqSliderValue + oscValue) > 20000) {
						oscValue = -(20000 - freqSliderValue)*0.9f;
					}
					//apply modulation to filter
					lowpassFilter.setFc((freqSliderValue + oscValue) / sampleRateInt);
					lowpassFilter2.setFc((freqSliderValue + oscValue) / sampleRateInt);
					lowpassFilter3.setFc((freqSliderValue + oscValue) / sampleRateInt);
					lowpassFilter4.setFc((freqSliderValue + oscValue) / sampleRateInt);

				}
			}
		}
    }

    void releaseResources() override
    {
		Logger::getCurrentLogger()->writeToLog("Releasing audio resources");
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (Colour(0x4D7C8C));


        // You can add your drawing code here!
    }

    void resized() override
    {

		volumeLabel.setBoundsRelative(.1f, .0f, .8f, .1f);
		volumeSlider.setBoundsRelative(.1f, .1f, .8f, .1f);
		freqLabel.setBoundsRelative(.1f, .2f, .8f, .1f);
		freqSlider.setBoundsRelative(.1f, .3f, .8f, .1f);
		fbLabel.setBoundsRelative(.1f, .4f, .8f, .1f);
		fbSlider.setBoundsRelative(.1f, .5f, .8f, .1f);
		modaLabel.setBoundsRelative(.1f, .6f, .8f, .1f);
		modaSlider.setBoundsRelative(.1f, .7f, .8f, .1f);
		modfLabel.setBoundsRelative(.1f, .8f, .8f, .1f);
		modfSlider.setBoundsRelative(.1f, .9f, .8f, .1f);
    }

	//saturating math
	float satAdd(float x, float y) {                 //x + y
		float MIN = -1.0f;
		float MAX = 1.0f;
		if (x < (MIN - y))                           //if((x + y) < MIN)
			return MIN;
		if (x >(MAX - y))                           //if((x + y) > MAX)
			return MAX;
		return (x + y);                             //if it's not limiting then just return x + y
	}

	float satSubtract(float x, float y) {            //x - y
		float MIN = -1.0f;
		float MAX = 1.0f;
		if (x < (MIN + y))                           //if((x - y) < MIN), add y to both sides to yields if(x < (MIN + y))
			return MIN;
		if (-y >(MAX - x))                          //if((x - y) > MAX), subtract x from both sides to yield if(-y > (MAX - x)) 
			return MAX;
		return (x - y);                             //if it's not limiting then just return x - y
	}

private:
    //==============================================================================

    // Your private member variables go here...

	//put the Random instantiation here.   Not sure..
	Random random;
	Slider volumeSlider, freqSlider, fbSlider, modfSlider, modaSlider;
	Label volumeLabel, freqLabel, fbLabel, modfLabel, modaLabel;
	float  volumeSliderValue, freqSliderValue, filterOut, filterFeedback, 
		fbSliderValue, filterOut2, modfSliderValue, modaSliderValue,
		oscValue;
	int sampleRateInt, initialShitHack = 0, initialShitHack2 = 0;
	OnePole lowpassFilter, lowpassFilter2, lowpassFilter3, lowpassFilter4;
	OnePoleHp highpassFilter;
	gsOsc modOsc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
