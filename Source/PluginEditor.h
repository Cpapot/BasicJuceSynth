/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "EnvEditor.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicJuceSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
    public:
        BasicJuceSynthAudioProcessorEditor (BasicJuceSynthAudioProcessor&);
        ~BasicJuceSynthAudioProcessorEditor() override;

        //==============================================================================
        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        // This reference is provided as a quick way for your editor to
        // access the processor object that created it.
        BasicJuceSynthAudioProcessor&   audioProcessor;
        juce::MidiKeyboardState&        keyboardState;
        juce::MidiKeyboardComponent     keyboardComponent;

        std::unique_ptr<EnvEditor>      envEditor;

        juce::ComboBox                                                          unisonSelector;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> unisonAttachment;

        juce::ComboBox                                                          waveSelector;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveAttachment;

        juce::Slider                                                            unisonDetuneKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   unisonDetuneAttachment;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicJuceSynthAudioProcessorEditor)
};
