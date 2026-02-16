/*
  ==============================================================================

    EnvPreview.cpp
    Created: 16 Feb 2026 4:53:28pm
    Author:  coren

  ==============================================================================
*/

#include "EnvPreview.h"

EnvPreview::EnvPreview(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts)
{
    startTimerHz(30);
}

float EnvPreview::getParam(const char* id) const
{
    if (auto* v = apvts.getRawParameterValue(id)) return *v;
    return 0.0f;
}

void    EnvPreview::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float attack = getParam("ENV_ATTACK");
    float decay = getParam("ENV_DECAY");
    float sustain = getParam("ENV_SUSTAIN");
    float release = getParam("ENV_RELEASE");

    float totalTime = std::max(0.001f, attack + decay + release + sustainDisplayTime);
    auto timeToX = [&](float t) { return bounds.getX() + (t / totalTime) * bounds.getWidth(); };
    auto ampToY = [&](float amp) { return bounds.getBottom() - amp * bounds.getHeight(); };

    juce::Path p;
    p.startNewSubPath(timeToX(0.0f), ampToY(0.0f));
    p.lineTo(timeToX(attack), ampToY(1.0f));                   // attack
    p.lineTo(timeToX(attack + decay), ampToY(sustain));        // decay
    p.lineTo(timeToX(attack + decay + sustainDisplayTime), ampToY(sustain)); // sustain line
    p.lineTo(timeToX(totalTime), ampToY(0.0f));                // release
    g.setColour(juce::Colours::lightblue);
    g.strokePath(p, juce::PathStrokeType(2.0f));
}