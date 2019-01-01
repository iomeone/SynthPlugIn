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
    parameters(*this, nullptr, Identifier("AudioTree"), {
        
        /** declare our parameters */
        std::make_unique<AudioParameterFloat> (id_SynthGain, "Gain", NormalisableRange<float>(0.0f, 2.0f), 1.0f),
		
		std::make_unique<AudioParameterInt> (id_NumVoices, "# Voices", 1, 12, 3),
		std::make_unique<AudioParameterInt> (id_NumPartials, "# Partials", 1, 200, 8),
		std::make_unique<AudioParameterInt>(id_NumUnison, "# Unison", 1, 3, 1),
		std::make_unique<AudioParameterFloat>(id_DeTuneUnisonHz, "Detune Hz", NormalisableRange<float>(0.0f, 500.0f), 200.0f),
		std::make_unique<AudioParameterInt> (id_SamplesPerIncrement, "Samples/Inc", 1, 900, 1),

		std::make_unique<AudioParameterFloat> (id_EnvPreAttack, "Pre-Attack", NormalisableRange<float>(0.001f, 2.0f), 0.01f),
		std::make_unique<AudioParameterFloat> (id_EnvPreAttackDecay, "Pre-Decay", NormalisableRange<float>(0.001f, 2.0f), 0.1f),
        std::make_unique<AudioParameterFloat> (id_EnvAttack, "Attack", NormalisableRange<float>(0.001f, 2.0f), 0.015f),
        std::make_unique<AudioParameterFloat> (id_EnvDecay, "Decay", NormalisableRange<float>(0.001f, 10.0f), 2.0f),
        std::make_unique<AudioParameterFloat> (id_EnvSustain, "Sustain", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat> (id_EnvRelease, "Release", NormalisableRange<float>(0.001f, 4.0f), 0.02f),
        
		std::make_unique<AudioParameterBool> (id_DelOnOff, "On/Off", true),
		std::make_unique<AudioParameterFloat> (id_DelTime, "Time", NormalisableRange<float>(0.0f, 2.0f), 0.25f),
        std::make_unique<AudioParameterFloat> (id_DelFeedback, "Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.4f),
        std::make_unique<AudioParameterFloat> (id_DelPrePostMix, "Pre/Post", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
        std::make_unique<AudioParameterFloat> (id_DelDWMix, "Dry/Wet", NormalisableRange<float>(0.0f, 1.0f), 0.3f),

		std::make_unique<AudioParameterFloat>(id_Dummy1, "Time", NormalisableRange<float>(0.0f, 2.0f), 0.25f),
		std::make_unique<AudioParameterFloat>(id_Dummy2, "Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.4f),
		std::make_unique<AudioParameterFloat>(id_Dummy3, "Pre/Post", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>(id_Dummy4, "Dry/Wet", NormalisableRange<float>(0.0f, 1.0f), 0.3f),
		std::make_unique<AudioParameterFloat>(id_Dummy5, "Time", NormalisableRange<float>(0.0f, 2.0f), 0.25f),
		std::make_unique<AudioParameterFloat>(id_Dummy6, "Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.4f),
		std::make_unique<AudioParameterFloat>(id_Dummy7, "Pre/Post", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>(id_Dummy8, "Dry/Wet", NormalisableRange<float>(0.0f, 1.0f), 0.3f),
		std::make_unique<AudioParameterFloat>(id_Dummy9, "Time", NormalisableRange<float>(0.0f, 2.0f), 0.25f),
		std::make_unique<AudioParameterFloat>(id_Dummy10, "Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.4f),
		std::make_unique<AudioParameterFloat>(id_Dummy11, "Pre/Post", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>(id_Dummy12, "Dry/Wet", NormalisableRange<float>(0.0f, 1.0f), 0.3f),
		std::make_unique<AudioParameterFloat>(id_Dummy13, "Time", NormalisableRange<float>(0.0f, 2.0f), 0.25f),
		std::make_unique<AudioParameterFloat>(id_Dummy14, "Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.4f),
		std::make_unique<AudioParameterFloat>(id_Dummy15, "Pre/Post", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>(id_Dummy16, "Dry/Wet", NormalisableRange<float>(0.0f, 1.0f), 0.3f),
		std::make_unique<AudioParameterFloat>(id_Dummy17, "Time", NormalisableRange<float>(0.0f, 2.0f), 0.25f),
		std::make_unique<AudioParameterFloat>(id_Dummy18, "Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.4f),
		std::make_unique<AudioParameterFloat>(id_Dummy19, "Pre/Post", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>(id_Dummy20, "Dry/Wet", NormalisableRange<float>(0.0f, 1.0f), 0.3f),
    })
#endif
{
    updateNumVoices();

    parameters.addParameterListener(id_NumVoices, this);

	parameters.addParameterListener(id_DelOnOff, this);
	parameters.addParameterListener(id_DelTime, this);
	parameters.addParameterListener(id_DelFeedback, this);
	parameters.addParameterListener(id_DelPrePostMix, this);
	parameters.addParameterListener(id_DelDWMix, this);

	initializeParameters();
}

AudioPlugInAudioProcessor::~AudioPlugInAudioProcessor()
{
    parameters.removeParameterListener(id_NumVoices, this);

	parameters.removeParameterListener(id_DelOnOff, this);
	parameters.removeParameterListener(id_DelTime, this);
	parameters.removeParameterListener(id_DelFeedback, this);
	parameters.removeParameterListener(id_DelPrePostMix, this);
	parameters.removeParameterListener(id_DelDWMix, this);
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

	monoDelay.setSampleRate(lastSampleRate, 2);
	
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
    
	buffer.clear();
	mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	
	if (delayOnOff) {
		monoDelay.renderNextBlock(buffer, delayTime, delayFeedback, delayPrePostMix, delayDWMix);
	}

    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);
    
        // ..do something to the data...
    
        // mLofi[channel].renderNextBlock(buffer, buffer, buffer.getNumSamples());
    
        // mDistortion[channel].renderNextBlock(buffer, buffer.getNumSamples());
    
        // mFilter[channel].renderNextBlock(buffer, buffer.getNumSamples());
    //}
    
	
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

	auto state = parameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void AudioPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void AudioPlugInAudioProcessor::initializeParameters()
{
	
	delayOnOff = *parameters.getRawParameterValue(id_DelOnOff);
	delayDWMix = *parameters.getRawParameterValue(id_DelDWMix);
	delayPrePostMix = *parameters.getRawParameterValue(id_DelPrePostMix);
	delayTime = *parameters.getRawParameterValue(id_DelTime);
	delayFeedback = *parameters.getRawParameterValue(id_DelFeedback);

}
void AudioPlugInAudioProcessor::parameterChanged (const String& parameterID, float newValue)
{
    if(parameterID == id_NumVoices){
        updateNumVoices();
    }

	else if (parameterID == id_DelOnOff) {
		delayOnOff = newValue;
	}
	else if (parameterID == id_DelTime) {
		delayTime = newValue;
	}
	else if (parameterID == id_DelFeedback) {
		delayFeedback = newValue;
	}
	else if (parameterID == id_DelPrePostMix) {
		delayPrePostMix = newValue;
	}
	else if (parameterID == id_DelDWMix) {
		delayDWMix = newValue;
	}
}

void AudioPlugInAudioProcessor::updateNumVoices()
{
    const ScopedLock voiceLock (lock);
    
    auto voices = parameters.getRawParameterValue(id_NumVoices);
    
    mySynth.clearVoices();
    
    for (int i = 0; i < *voices; i++)
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

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlugInAudioProcessor();
}
