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
    attackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 12);
    attackKnob.setRange(0.001f, 2.0f, 0.001f);
    attackLabel.setText("Attack", juce::dontSendNotification);
    addAndMakeVisible(attackKnob);
    addAndMakeVisible(attackLabel);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_ATTACK", attackKnob);

    decayKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decayKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 12);
    decayKnob.setRange(0.001f, 2.0f, 0.001f);
    decayLabel.setText("Decay", juce::dontSendNotification);
    addAndMakeVisible(decayKnob);
    addAndMakeVisible(decayLabel);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_DECAY", decayKnob);

    sustainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 12);
    sustainKnob.setRange(0.0f, 1.0f, 1.0f);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    addAndMakeVisible(sustainKnob);
    addAndMakeVisible(sustainLabel);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_SUSTAIN", sustainKnob);

    releaseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 12);
    releaseKnob.setRange(0.001f, 5.0f, 0.001f);
    releaseLabel.setText("Release", juce::dontSendNotification);
    addAndMakeVisible(releaseKnob);
    addAndMakeVisible(releaseLabel);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ENV_RELEASE", releaseKnob);
}

void EnvEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack);
}

void EnvEditor::resized()
{
    auto area = getLocalBounds().reduced(6);
    auto row = area.removeFromTop(area.getHeight() * 0.5);

    const int knobW = juce::jmin(100, row.getWidth() / 4);
    attackKnob.setBounds(row.removeFromLeft(knobW).reduced(6));
    attackLabel.setBounds(attackKnob.getX(), attackKnob.getBottom(), attackKnob.getWidth(), 10);

    decayKnob.setBounds(row.removeFromLeft(knobW).reduced(6));
    decayLabel.setBounds(decayKnob.getX(), decayKnob.getBottom(), decayKnob.getWidth(), 10);

    sustainKnob.setBounds(row.removeFromLeft(knobW).reduced(6));
    sustainLabel.setBounds(sustainKnob.getX(), sustainKnob.getBottom(), sustainKnob.getWidth(), 10);

    releaseKnob.setBounds(row.removeFromLeft(knobW).reduced(6));
    releaseLabel.setBounds(releaseKnob.getX(), releaseKnob.getBottom(), releaseKnob.getWidth(), 10);
}