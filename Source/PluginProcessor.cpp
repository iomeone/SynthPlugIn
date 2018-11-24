/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

static const int numVoices = 6;

//==============================================================================
AudioPlugInAudioProcessor::AudioPlugInAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	parameters(*this, nullptr)
#endif
{
	initializeParameters();

	mySynth.clearVoices();

	for (int i = 0; i < numVoices; i++)
	{
		// new voice
		SynthVoice* voice = new SynthVoice(&parameters);

		// add voice to synth
		mySynth.addVoice(voice);

		// track voices in array
		mVoices.add(voice);
	}
	mySynth.clearSounds();
	mySynth.addSound(new SynthSound());

}

AudioPlugInAudioProcessor::~AudioPlugInAudioProcessor()
{
}

//==============================================================================
const String AudioPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlugInAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPlugInAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	ignoreUnused(samplesPerBlock);
	lastSampleRate = sampleRate;
	mySynth.setCurrentPlaybackSampleRate(lastSampleRate);
	

}

void AudioPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioPlugInAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
	auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
	//for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
		//buffer.clear();
	//	auto* channelData = buffer.getWritePointer (channel);
	
        // ..do something to the data...
    //}
	
	buffer.clear();
	mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	
}

//==============================================================================
bool AudioPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPlugInAudioProcessor::createEditor()
{
    return new AudioPlugInAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPlugInAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void AudioPlugInAudioProcessor::initializeParameters()
{ // parameter ID, parameter name, parameter label (suffix), range, default value, null, null


	parameters.createAndAddParameter(id_SynthGain, "Synth Gain", String(), NormalisableRange<float>(0.0f, 2.0f), 1.0f, nullptr, nullptr);
	
	parameters.createAndAddParameter(id_EnvAttack, "Envelope Attack", String(), NormalisableRange<float>(0.001f, 2.0f), 0.01f, nullptr, nullptr);
	parameters.createAndAddParameter(id_EnvDecay, "Envelope Decay", String(), NormalisableRange<float>(0.001f, 10.0f), 3.0f, nullptr, nullptr);
	parameters.createAndAddParameter(id_EnvSustain, "Envelope Sustain", String(), NormalisableRange<float>(0.0f, 1.0f), 0.0f, nullptr, nullptr);
	parameters.createAndAddParameter(id_EnvRelease, "Envelope Release", String(), NormalisableRange<float>(0.001f, 4.0f), 0.02f, nullptr, nullptr);

//    AudioParameterFloat (const String& parameterID,
//                         const String& name,
//                         NormalisableRange<float> normalisableRange,
//                         float defaultValue,
//                         const String& label = String(),
//                         Category category = AudioProcessorParameter::genericParameter,
//                         std::function<String (float value, int maximumStringLength)> stringFromValue = nullptr,
//                         std::function<float (const String& text)> valueFromString = nullptr);
    
    
//    AudioParameterFloat* oscillatorGain = new AudioParameterFloat ("test",
//                                                              "Synth Gain",
//                                                              NormalisableRange<float>(-24.0f, 24.0f),
//                                                              0.5f,
//                                                              String(),
//                                                              AudioProcessorParameter::genericParameter,
//                                                              nullptr,
//                                                              nullptr);
//
//    std::unique_ptr<AudioParameterFloat> gain = std::unique_ptr<AudioParameterFloat>(oscillatorGain);
    
    
//    parameters.createAndAddParameter(gain);
    
	/*
	for(int i = 0; i < kParameterId_TotalNumParamters; i ++){
	parameters.createAndAddParameter(ParameterIDs[i],
	ParameterIDs[i],
	ParameterIDs[i], NormalisableRange<float>(0.0, 1.0), 0.5f, nullptr, nullptr);
	}*/

	parameters.state = ValueTree(Identifier("Mike"));
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlugInAudioProcessor();
}
