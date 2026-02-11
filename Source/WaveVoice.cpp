#include "WaveVoice.h"

void	WaveVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	std::cout << currentWaveType << std::endl;
	phaseIncrement = (frequency) / getSampleRate();
	if (amplitude > 0.0f) {
		for (int sample = 0; sample < numSamples; ++sample)
		{
			float sampleValue = 0.0f;
			switch (currentWaveType) {
				case Sine:     sampleValue = (std::sin(phase * 2.0f * juce::MathConstants<float>::pi)) * amplitude; break;
				case Saw:      sampleValue = ((2.0f * phase) - 1.0f) * amplitude; break;
				case Square:   sampleValue = ((phase < 0.5f) ? 0.5f : -0.5f) * amplitude; break;
				case Triangle: sampleValue = (2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f) * amplitude; break;
				case Noise:    sampleValue = ((juce::Random::getSystemRandom().nextFloat() * 2.0f) - 1.0f) * amplitude; break;
			}
			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
				outputBuffer.addSample(channel, startSample + sample, sampleValue);
			}
			phase += phaseIncrement;
			if (phase >= 1.0f)
				phase -= 1.0f;
		}
	}
}
