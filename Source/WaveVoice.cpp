#include "WaveVoice.h"
#include <cmath>

void WaveVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	// simple unison rendering: mix all active unison voices and write to output
	if (amplitude <= 0.0f)
		return;

	const int numChannels = outputBuffer.getNumChannels();
	const int numVoices = juce::jlimit (1, MAX_UNISON, currentActiveUnisonVoices);

	// Precompute per-voice linear interpolation step for phaseIncrement
	// (step = (target - current) / numSamples)
	float steps[MAX_UNISON] = {};
	for (int v = 0; v < numVoices; ++v)
		steps[v] = (phaseIncrementTargets[v] - phaseIncrements[v]) / static_cast<float>(juce::jmax(1, numSamples));

	for (int sample = 0; sample < numSamples; ++sample)
	{
		float leftSum = 0.0f;
		float rightSum = 0.0f;

		for (int v = 0; v < numVoices; ++v)
		{
			phaseIncrements[v] += steps[v];

			const float ph = phases[v];
			const float phInc = phaseIncrements[v];

			float voiceSample = 0.0f;

			switch (currentWaveType)
			{
				case Sine:     voiceSample = std::sin (ph * juce::MathConstants<float>::twoPi) * amplitude; break;
				case Saw:      voiceSample = ((2.0f * ph) - 1.0f) * amplitude; break;
				case Square:   voiceSample = (ph < 0.5f) ? (0.5f * amplitude) : (-0.5f * amplitude); break;
				case Triangle: voiceSample = (2.0f * std::abs (2.0f * ph - 1.0f) - 1.0f) * amplitude; break;
				case Noise:    voiceSample = ((juce::Random::getSystemRandom().nextFloat() * 2.0f) - 1.0f) * amplitude; break;
			}

			// compute symmetric detuneFactor in [-1, 1] to determine stereo position
			float detuneFactor = 0.0f;
			if (numVoices > 1)
				detuneFactor = (static_cast<float> (v) / static_cast<float> (numVoices - 1)) * 2.0f - 1.0f;

			// detuneParam acts as pan spread (0.0 = center, 1.0 = full spread)
			const float panSpread = juce::jlimit(0.0f, 1.0f, detuneParam);
			const float pan = 0.5f + detuneFactor * 0.5f * panSpread; // [0..1] where 0 = left, 1 = right

			// constant-power panning
			const float angle = pan * (juce::MathConstants<float>::halfPi);
			const float leftGain = std::cos(angle);
			const float rightGain = std::sin(angle);

			leftSum += voiceSample * leftGain;
			rightSum += voiceSample * rightGain;

			// advance phase for this unison voice
			phases[v] = ph + phInc;
			if (phases[v] >= 1.0f)
				phases[v] -= 1.0f;
			else if (phases[v] < 0.0f)
				phases[v] += 1.0f;
		}

		// normalize using constant-power normalization (1/sqrt(N)) to preserve perceived loudness
		const float invSqrt = 1.0f / std::sqrt(static_cast<float> (numVoices));
		leftSum *= invSqrt;
		rightSum *= invSqrt;

		// apply a tiny attack envelope if requested to prevent clicks
		if (attackSamplesRemaining > 0)
		{
			const float env = 1.0f - (static_cast<float> (attackSamplesRemaining) / static_cast<float> (attackLengthSamples));
			leftSum *= env;
			rightSum *= env;
			--attackSamplesRemaining;
		}

		// deliver to output channels: mono fallback mixes L+R, stereo uses L and R
		if (numChannels == 1)
		{
			const float mono = 0.5f * (leftSum + rightSum);
			outputBuffer.addSample(0, startSample + sample, mono);
		}
		else
		{
			// channel 0 = left, channel 1 = right
			outputBuffer.addSample(0, startSample + sample, leftSum);
			outputBuffer.addSample(1, startSample + sample, rightSum);

			// For any extra channels, write a center mix
			if (numChannels > 2)
			{
				const float center = 0.5f * (leftSum + rightSum);
				for (int ch = 2; ch < numChannels; ++ch)
					outputBuffer.addSample(ch, startSample + sample, center);
			}
		}
	}
	for (int v = 0; v < numVoices; ++v)
		phaseIncrements[v] = phaseIncrementTargets[v];
}
