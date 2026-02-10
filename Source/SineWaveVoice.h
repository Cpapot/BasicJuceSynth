#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

struct SineWaveSound : public juce::SynthesiserSound {
	SineWaveSound() {}
	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

class SineWaveVoice : public juce::SynthesiserVoice
{
	public:
		bool	canPlaySound(juce::SynthesiserSound* sound) override;
		void	startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
		void	stopNote(float /*velocity*/, bool allowTailOff) override;
		void	renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

		void	pitchWheelMoved(int) override {}
		void	controllerMoved(int, int) override {}

	private:
		float frequency = 0.0f;
		float amplitude = 0.0f;
		float phase = 0.0f;
		float phaseIncrement = 0.0f;
};

