/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthVoice.h"
#include "SynthSound.h"
#include "Synthesiser.h"

//==============================================================================
/**
*/
class AudioPlugInAudioProcessor
:   public AudioProcessor,
    public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    AudioPlugInAudioProcessor();
    ~AudioPlugInAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void initializeParameters();
    void parameterChanged (const String& parameterID, float newValue) override;

	// Our parameters
	AudioProcessorValueTreeState parameters;

private:
    //==============================================================================
    
    CriticalSection lock;
    
    /** internal */
    void updateNumVoices();

	Array<SynthVoice*> mVoices;
	SynthesiserInherited mySynth;

	MaxiDelayline monoDelay;

	bool monoDOnOff = false;
	bool delayOnOff = false;
	float delayTime = 0.0f;
	float delayPrePostMix = 1.0f;
	float delayDWMix = 0.7f;
	float delayFeedback = 0.4f;

	double lastSampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlugInAudioProcessor)
};
