/*
  ==============================================================================

    EnvEditor.cpp
    Created: 16 Feb 2026 1:16:00pm
    Author:  coren

  ==============================================================================
*/

#include "EnvEditor.h"

EnvEditor::EnvEditor(juce::AudioProcessorValueTreeState& state) : apvts(state)
{
    envKnobs = std::make_unique<EnvKnobs>(apvts);
    addAndMakeVisible(envKnobs.get());

    envPreview = std::make_unique<EnvPreview>(apvts);
    addAndMakeVisible(envPreview.get());
}

void EnvEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkblue);
}

void EnvEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(5);
    envPreview->setBounds(area.removeFromTop(area.getHeight() * 0.7));
    envKnobs->setBounds(area);
}