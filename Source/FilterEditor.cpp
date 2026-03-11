/*
  ==============================================================================

    FilterEditor.cpp
    Created: 9 Mar 2026 10:34:56am
    Author:  coren

  ==============================================================================
*/

#include "FilterEditor.h"

FilterEditor::FilterEditor(juce::AudioProcessorValueTreeState& state) : apvts(state)
{
    filterSelector.addItem("High12", 1);
    filterSelector.addItem("High24", 2);
    filterSelector.addItem("Low12", 3);
    filterSelector.addItem("Low24", 4);
    filterSelector.addItem("Band12", 5);
    filterSelector.addItem("Band24", 6);
    addAndMakeVisible(filterSelector);
    filterAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "FILTER_TYPE", filterSelector);

    cutOffKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    cutOffKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 15);
    cutOffKnob.setRange(20.0f, 20000.0f, 1.0f);
    cutOffLabel.setText("CutOff", juce::dontSendNotification);
    addAndMakeVisible(cutOffKnob);
    addAndMakeVisible(cutOffLabel);
    cutOffAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FILTER_CUTOFF", cutOffKnob);

    resKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    resKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 15);
    resKnob.setRange(0.01f, 10.0f, 0.001f);
    resLabel.setText("Res", juce::dontSendNotification);
    addAndMakeVisible(resKnob);
    addAndMakeVisible(resLabel);
    resAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FILTER_RES", resKnob);

    //filterBypassButton.set
    addAndMakeVisible(filterBypassButton);
    filterBypassAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "FILTER_ENABLED", filterBypassButton);

    filterPreview = std::make_unique<FilterPreview>(apvts);
    addAndMakeVisible(filterPreview.get());
}

void FilterEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void FilterEditor::resized()
{
    auto area = getLocalBounds().reduced(6);
    filterSelector.setBounds(5, 5, area.getWidth() - 50, 40 );
    filterBypassButton.setBounds(area.getWidth() - 25 ,5 ,40 ,40);
    area.removeFromTop(50);

    filterPreview->setBounds(area.removeFromTop(70));

    const int cellW = area.getWidth() / 2;
    const int labelH = 14;
    const int availH = area.getHeight();
    const int knobH = juce::jlimit(45, 120, static_cast<int> (availH * 0.6f));

    for (int i = 0; i < 2; ++i)
    {
        auto cell = area.removeFromLeft(cellW);
        const int kW = juce::jmin(cellW - 12, knobH);
        const int kX = cell.getX() + (cell.getWidth() - kW) / 2;
        const int kY = cell.getY() + (cell.getHeight() - (kW + labelH)) / 2;
        switch (i)
        {
            case 0:
                cutOffKnob.setBounds(kX, kY, kW, kW);
                cutOffLabel.setBounds(kX, kY + kW, kW, labelH);
                break;
            case 1:
                resKnob.setBounds(kX, kY, kW, kW);
                resLabel.setBounds(kX, kY + kW, kW, labelH);
                break;
        }
    }
}
