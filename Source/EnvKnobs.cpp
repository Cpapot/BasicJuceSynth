/*
  ==============================================================================

    EnvKnobs.cpp
    Created: 16 Feb 2026 5:33:40pm
    Author:  coren

  ==============================================================================
*/

#include "EnvKnobs.h"

EnvKnobs::EnvKnobs(juce::AudioProcessorValueTreeState& state) : apvts(state)
{
    attackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 15);
    attackKnob.setRange(0.001f, 2.0f, 0.001f);
    attackLabel.setText("Attack", juce::dontSendNotification);
    addAndMakeVisible(attackKnob);
    addAndMakeVisible(attackLabel);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_ATTACK", attackKnob);

    decayKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decayKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 15);
    decayKnob.setRange(0.001f, 2.0f, 0.001f);
    decayLabel.setText("Decay", juce::dontSendNotification);
    addAndMakeVisible(decayKnob);
    addAndMakeVisible(decayLabel);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_DECAY", decayKnob);

    sustainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 15);
    sustainKnob.setRange(0.0f, 1.0f, 1.0f);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    addAndMakeVisible(sustainKnob);
    addAndMakeVisible(sustainLabel);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_SUSTAIN", sustainKnob);

    releaseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 15);
    releaseKnob.setRange(0.001f, 5.0f, 0.001f);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setAlwaysOnTop(true);
    addAndMakeVisible(releaseKnob);
    addAndMakeVisible(releaseLabel);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_RELEASE", releaseKnob);
}

void EnvKnobs::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void EnvKnobs::resized()
{
    auto area = getLocalBounds().reduced (6);

    // Reserve top half for preview in parent; here we use full area passed to EnvKnobs.
    // Layout four knob cells horizontally.
    const int cellW = area.getWidth() / 4;
    const int labelH = 14;

    // Choose knob height as a ratio of the available height, with sensible min/max
    const int availH = area.getHeight();
    const int knobH = juce::jlimit (45, 120, static_cast<int> (availH * 0.6f));

    for (int i = 0; i < 4; ++i)
    {
        auto cell = area.removeFromLeft (cellW);

        const int kW = juce::jmin (cellW - 12, knobH);
        const int kX = cell.getX() + (cell.getWidth() - kW) / 2;
        const int kY = cell.getY() + (cell.getHeight() - (kW + labelH)) / 2;

        switch (i)
        {
            case 0:
                attackKnob.setBounds (kX, kY, kW, kW);
                attackLabel.setBounds (kX, kY + kW, kW, labelH);
                break;
            case 1:
                decayKnob.setBounds (kX, kY, kW, kW);
                decayLabel.setBounds (kX, kY + kW, kW, labelH);
                break;
            case 2:
                sustainKnob.setBounds (kX, kY, kW, kW);
                sustainLabel.setBounds (kX, kY + kW, kW, labelH);
                break;
            case 3:
                releaseKnob.setBounds (kX, kY, kW, kW);
                releaseLabel.setBounds (kX, kY + kW, kW, labelH);
                break;
        }
    }
}