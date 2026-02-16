/*
  ==============================================================================

    EnvKnobs.h
    Created: 16 Feb 2026 5:33:40pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class EnvKnobs : public juce::Component
{
public:
    EnvKnobs(juce::AudioProcessorValueTreeState& state);
    ~EnvKnobs() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    juce::AudioProcessorValueTreeState& apvts;

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
};