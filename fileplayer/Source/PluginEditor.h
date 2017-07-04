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
class FileplayerAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener, public ChangeListener
{
public:
    FileplayerAudioProcessorEditor (FileplayerAudioProcessor&);
    ~FileplayerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* button) override
	{
		if (button == &openButton)  openButtonClicked();
		if (button == &playButton)  playButtonClicked();
		if (button == &stopButton)  stopButtonClicked();
	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (source == &processor.transportSource)
		{
			if (processor.transportSource.isPlaying())
				FileplayerAudioProcessorEditor::changeState(FileplayerAudioProcessorEditor::Playing);
			else
				FileplayerAudioProcessorEditor::changeState(FileplayerAudioProcessorEditor::Stopped);
		}
	}

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FileplayerAudioProcessor& processor;

	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;

	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};

	TransportState state;

	void changeState(TransportState newState);
	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileplayerAudioProcessorEditor)
};
