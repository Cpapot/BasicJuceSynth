/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WaveVoice.h"

//==============================================================================
static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    using namespace juce;

    AudioProcessorValueTreeState::ParameterLayout layout;

    // Wave type parameter (used by the voices)
    StringArray waveChoices { "Sine", "Saw", "Square", "Triangle", "Noise" };
    layout.add (std::make_unique<AudioParameterChoice> ("WAVE_TYPE", "Wave Type", waveChoices, 0));

    // Add more parameters here as needed. Example:
    // layout.add(std::make_unique<AudioParameterFloat>(
    //     "gain",
    //     "Gain",
    //     juce::NormalisableRange<float>(0.0f, 1.0f),
    //     0.5f));
    
    return layout;
}

//==============================================================================
BasicJuceSynthAudioProcessor::BasicJuceSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
     :
#endif
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    for (int i = 0; i < 16; ++i) {
        synth.addVoice(new WaveVoice());
    }
    synth.addSound(new VoiceSound());
}

BasicJuceSynthAudioProcessor::~BasicJuceSynthAudioProcessor()
{
}

//==============================================================================
const juce::String BasicJuceSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicJuceSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicJuceSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicJuceSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicJuceSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicJuceSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicJuceSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicJuceSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicJuceSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicJuceSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicJuceSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void BasicJuceSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicJuceSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void BasicJuceSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    buffer.clear();

    int selectedType = 0;
    if (auto* p = apvts.getRawParameterValue ("WAVE_TYPE"))
        selectedType = juce::jlimit (0, 4, (int) std::lround (*p));

    // Mettre à jour toutes les voix du synthé
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
            if (auto* v = dynamic_cast<WaveVoice*>(synth.getVoice(i)))
                v->setWaveType (selectedType);
    }

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool BasicJuceSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicJuceSynthAudioProcessor::createEditor()
{
    return new BasicJuceSynthAudioProcessorEditor (*this);
}

//==============================================================================
void BasicJuceSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicJuceSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicJuceSynthAudioProcessor();
}
