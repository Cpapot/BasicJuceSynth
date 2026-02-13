/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveVoice.h"

//==============================================================================
/**
*/
class BasicJuceSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BasicJuceSynthAudioProcessor();
    ~BasicJuceSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Provide access to the processor's MidiKeyboardState for the editor
    juce::MidiKeyboardState& getKeyboardState() noexcept { return keyboardState; }

    juce::AudioProcessorValueTreeState                  apvts;


private:
    juce::Synthesiser       synth;
    juce::MidiKeyboardState keyboardState;

    // add near existing private members
    float smoothedDetune = 0.0f;
    bool  smoothedDetuneInitialized = false;
    float lastSentDetune = 0.0f;
    // smoothing time constant (seconds) — increase to make knob slower / smoother
    float detuneTauSeconds = 0.001f;
    // deadband (cents) to avoid tiny changes producing audible artifacts
    float detuneUpdateThreshold = 0.5f;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicJuceSynthAudioProcessor)
};