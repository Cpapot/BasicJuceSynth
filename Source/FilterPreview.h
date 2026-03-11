/*
  ==============================================================================

    FilterPreview.h
    Created: 11 Mar 2026 12:58:17pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterPreview : public juce::Component, private juce::Timer
{
    public:
        FilterPreview(juce::AudioProcessorValueTreeState& apvts);

        void paint(juce::Graphics& g) override;
        void timerCallback() override { repaint(); }

    private:
        float getParam(const char* id) const;
        float xToFreq(float x, float width);
        float magToY(float mag, float height);

        juce::AudioProcessorValueTreeState& apvts;
};