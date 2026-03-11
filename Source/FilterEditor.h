/*
  ==============================================================================

    FilterEditor.h
    Created: 9 Mar 2026 10:34:56am
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterPreview.h"

class FilterEditor : public juce::Component
{
    public:
        FilterEditor(juce::AudioProcessorValueTreeState& apvts);
        ~FilterEditor() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:
        std::unique_ptr<FilterPreview>                                          filterPreview;

        juce::AudioProcessorValueTreeState&                                     apvts;

        juce::ComboBox                                                          filterSelector;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterAttachement;

        juce::ToggleButton                                                      filterBypassButton;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   filterBypassAttachement;

        juce::Label                                                             cutOffLabel;
        juce::Slider                                                            cutOffKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   cutOffAttachement;

        juce::Label                                                             resLabel;
        juce::Slider                                                            resKnob;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   resAttachement;
};