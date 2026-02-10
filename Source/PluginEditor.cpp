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
}

BasicJuceSynthAudioProcessorEditor::~BasicJuceSynthAudioProcessorEditor()
{
}

//==============================================================================
void BasicJuceSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World! ok oui super", getLocalBounds(), juce::Justification::centred, 1);
    keyboardComponent.setBounds(20, 410, getWidth() - 40, 50);
}

void BasicJuceSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
