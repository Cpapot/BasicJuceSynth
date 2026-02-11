/*
  ==============================================================================

    AbstractVoice.cpp
    Created: 11 Feb 2026 5:39:03pm
    Author:  coren

  ==============================================================================
*/

#include "AbstractVoice.h"

bool	AbstractVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<VoiceSound*> (sound) != nullptr;
}

void	AbstractVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    clearCurrentNote();
    amplitude = 0.0f;
}

void	AbstractVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    amplitude = velocity * 0.15f;
    phase = 0.0f;
}