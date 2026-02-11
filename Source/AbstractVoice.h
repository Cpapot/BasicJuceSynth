/*
  ==============================================================================

    AbstractVoice.h
    Created: 11 Feb 2026 5:39:03pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

enum WaveType {
    Sine = 0,
    Saw,
    Square,
    Triangle,
    Noise
};

struct VoiceSound : public juce::SynthesiserSound {
    VoiceSound() {}
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

class AbstractVoice : public juce::SynthesiserVoice
{
    public:
        bool	        canPlaySound(juce::SynthesiserSound* sound) override;
        void	        stopNote(float /*velocity*/, bool allowTailOff) override;
        void	        startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;


        void	        pitchWheelMoved(int) override {}
        void	        controllerMoved(int, int) override {}

    protected:
        float frequency = 0.0f;
        float amplitude = 0.0f;
        float phase = 0.0f;
        float phaseIncrement = 0.0f;
};