/*
  ==============================================================================

    EnvEditor.h
    Created: 16 Feb 2026 1:16:00pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class EnvEditor : public juce::Component
{
    public:
        EnvEditor(juce::AudioProcessorValueTreeState& state);
        ~EnvEditor() override = default;

        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        juce::AudioProcessorValueTreeState&                                     apvts;

        juce::Label                                                             attackLabel;
        juce::Slider                                                            attackKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   attackAttachment;

        juce::Label                                                             decayLabel;
        juce::Slider                                                            decayKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   decayAttachment;

        juce::Label                                                             sustainLabel;
        juce::Slider                                                            sustainKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   sustainAttachment;

        juce::Label                                                             releaseLabel;
        juce::Slider                                                            releaseKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   releaseAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvEditor)
};