/*
  ==============================================================================

    SineWaveChannel.h
    Created: 10 Feb 2026 6:47:03pm
    Author:  coren

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <numbers>

class SineWaveChannel
{
public:
    void prepare(double sampleRate);
    void process(float* output, int numSamples);

    float getAmplitude() const { return amplitude; }
    float getFrequency() const { return frequency; }
    void setAmplitude(const float newAmplitude) { amplitude = newAmplitude; }
    void setFrequency(const float newFrequency) { frequency = newFrequency; }

private:
    float amplitude = 0.2f;
    float frequency = 440.0f;
    float currentSampleRate = 0.0f;
    float timeIncrement = 0.0f;
    float currentTime = 0.0f;
};