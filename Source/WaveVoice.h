#pragma once

#include "AbstractVoice.h"

class WaveVoice : public AbstractVoice
{
	public:
		void	renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

		void	setWaveType(int type) { currentWaveType = type; }

	private:
		int		currentWaveType = Sine;

};