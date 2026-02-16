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
    StringArray unisonChoice{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
    layout.add(std::make_unique<AudioParameterChoice>("UNISON_COUNT", "Unison Count", unisonChoice, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("UNISON_DETUNE", "Detune",
        juce::NormalisableRange<float>(0.0f, 100.0f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ENV_ATTACK", "Attack",
        juce::NormalisableRange<float>(0.001f, 2.0f, 0.001f, 0.2f), 0.001f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ENV_DECAY", "Decay",
        juce::NormalisableRange<float>(0.001f, 2.0f, 0.001f, 0.2f), 2.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ENV_SUSTAIN", "Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ENV_RELEASE", "Release",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.2f), 0.001f));    
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
    synth.clearSounds();
    synth.clearVoices();
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

    int unisonCount = 1;
    if (auto* p = apvts.getRawParameterValue("UNISON_COUNT"))
        unisonCount = juce::jlimit(0, 15, (int)std::lround(*p));

    float detuneValue = 0.5f;
    if (auto* p = apvts.getRawParameterValue("UNISON_DETUNE"))
        detuneValue = *p;

    float attack, decay, sustain, release;
    if (auto* p = apvts.getRawParameterValue("ENV_ATTACK"))
        attack = *p;
    if (auto* p = apvts.getRawParameterValue("ENV_DECAY"))
        decay = *p;
    if (auto* p = apvts.getRawParameterValue("ENV_SUSTAIN"))
        sustain = *p;
    if (auto* p = apvts.getRawParameterValue("ENV_RELEASE"))
        release = *p;
    


    const double sr = getSampleRate();
    const int numSamples = buffer.getNumSamples();

    if (!smoothedDetuneInitialized)
    {
        smoothedDetune = detuneValue;
        smoothedDetuneInitialized = true;
    }

    // compute block alpha: alphaBlock = 1 - exp(-numSamples / (tau * sr))
    const float alphaBlock = 1.0f - std::exp(-static_cast<float>(buffer.getNumSamples()) / (detuneTauSeconds * (float)getSampleRate()));
    smoothedDetune += alphaBlock * (detuneValue - smoothedDetune);

    if (std::fabs(smoothedDetune - lastSentDetune) > detuneUpdateThreshold)
    {
        for (auto i = 0; i < synth.getNumVoices(); ++i)
            if (auto* v = dynamic_cast<WaveVoice*>(synth.getVoice(i)))
                v->updateDetuneParam(smoothedDetune);
        lastSentDetune = smoothedDetune;
    }

    // still need to update wave type and unison count each block (no detune change)
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* v = dynamic_cast<WaveVoice*>(synth.getVoice(i)))
        {
            v->setWaveType(selectedType);
            v->setCurrentActiveUnisonVoices(unisonCount + 1);
            v->updateADSR(attack, decay, sustain, release);
        }
    }

    // process MIDI + render
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
