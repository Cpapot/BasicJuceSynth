/*
  ==============================================================================

    EnvEditor.h
    Created: 16 Feb 2026 1:16:00pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EnvKnobs.h"
#include "EnvPreview.h"

class EnvEditor : public juce::Component
{
    public:
        EnvEditor(juce::AudioProcessorValueTreeState& apvts);
        ~EnvEditor() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:
        std::unique_ptr<EnvKnobs>           envKnobs;
        std::unique_ptr<EnvPreview>         envPreview;

        juce::AudioProcessorValueTreeState& apvts;
};