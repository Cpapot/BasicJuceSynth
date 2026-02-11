/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicJuceSynthAudioProcessorEditor::BasicJuceSynthAudioProcessorEditor (BasicJuceSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardState (p.getKeyboardState()), keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible (keyboardComponent);
    keyboardComponent.setAvailableRange(36, 36 + 71);
    keyboardComponent.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, juce::Colours::cyan.withAlpha(0.5f));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colours::white.withAlpha(0.2f));
    setSize (720, 480);

    addAndMakeVisible(waveSelector);
    waveSelector.addItem("Sine", 1);
    waveSelector.addItem("Saw", 2);
    waveSelector.addItem("Square", 3);
    waveSelector.addItem("Triangle", 4);
    waveSelector.addItem("Noise", 5);
    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "WAVE_TYPE", waveSelector);
}

BasicJuceSynthAudioProcessorEditor::~BasicJuceSynthAudioProcessorEditor()
{
}

//==============================================================================
void BasicJuceSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


    keyboardComponent.setBounds(20, 410, getWidth() - 40, 50);
    waveSelector.setBounds(20, 20, 100, 30);
}

void BasicJuceSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
