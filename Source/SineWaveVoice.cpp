#include "SineWaveVoice.h"

bool	SineWaveVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<SineWaveSound*> (sound) != nullptr;
}

void	SineWaveVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
	frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	amplitude = velocity * 0.15f;
	phase = 0.0f;
	phaseIncrement = (juce::MathConstants<float>::twoPi * frequency) / getSampleRate();
}

void	SineWaveVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
	clearCurrentNote();
	amplitude = 0.0f;
}

void	SineWaveVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (amplitude > 0.0f) {
		for (int sample = 0; sample < numSamples; ++sample)
		{
			float sampleValue = std::sin(phase) * amplitude;
			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
				outputBuffer.addSample(channel, startSample + sample, sampleValue);
			}
			phase += phaseIncrement;
		}
	}
}
