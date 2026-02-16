/*
  ==============================================================================

    EnvPreview.h
    Created: 16 Feb 2026 4:53:28pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class EnvPreview : public juce::Component, private juce::Timer
{
    public:
        EnvPreview(juce::AudioProcessorValueTreeState& apvts);

        void paint(juce::Graphics& g) override;
        void timerCallback() override { repaint(); }

    private:
        float getParam(const char* id) const;

        juce::AudioProcessorValueTreeState& apvts;
        const float                         sustainDisplayTime = 0.2f;
};