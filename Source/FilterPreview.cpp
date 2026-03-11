/*
  ==============================================================================

    FilterPreview.cpp
    Created: 11 Mar 2026 12:58:17pm
    Author:  coren

  ==============================================================================
*/

#include "FilterPreview.h"

FilterPreview::FilterPreview(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts)
{
    startTimerHz(60);
}

float   FilterPreview::getParam(const char* id) const
{
    if (auto* v = apvts.getRawParameterValue(id)) return *v;
    return 0.0f;
}

float   FilterPreview::xToFreq(float x, float width) {
    return 20.0f * std::pow(1000.0f, x / width);
}

float   FilterPreview::magToY(float mag, float height) {
    // On considère que 1.0 (0dB) est au milieu ou un peu plus bas
    float y = height - (mag * height * 0.4f);
    return juce::jlimit(0.0f, height, y);
}

void    FilterPreview::paint(juce::Graphics& g)
{
    float cutoff = getParam("FILTER_CUTOFF");
    float res = getParam("FILTER_RES");
    int filterTypeIndex = static_cast<int>(apvts.getRawParameterValue("FILTER_TYPE")->load());

    juce::Path responseCurve;
    const auto area = getLocalBounds().toFloat();
    const float width = area.getWidth();
    const float height = area.getHeight();

    for (int x = 0; x <= width; x += 2)
    {
        float f = xToFreq(static_cast<float>(x), width);
        float omega = f / cutoff;

        float denominator = std::sqrt(std::pow(1.0f - std::pow(omega, 2.0f), 2.0f) + std::pow(omega / res, 2.0f));
        float magnitude = 0.0f;

        if (filterTypeIndex == 0 || filterTypeIndex == 1) // HighPass
            magnitude = std::pow(omega, 2.0f) / denominator;
        else if (filterTypeIndex == 2 || filterTypeIndex == 3) // LowPass
            magnitude = 1.0f / denominator;
        else if (filterTypeIndex == 4 || filterTypeIndex == 5) // BandPass
            magnitude = (omega / res) / denominator;

        if (filterTypeIndex % 2 != 0)
            magnitude = std::pow(magnitude, 2.0f);

        float y = magToY(magnitude, height);

        if (x == 0) responseCurve.startNewSubPath(0, y);
        else responseCurve.lineTo(static_cast<float>(x), y);
    }

    g.setColour(juce::Colours::cyan);
    g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
}
