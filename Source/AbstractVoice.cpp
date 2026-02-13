/*
  ==============================================================================

    AbstractVoice.cpp
    Created: 11 Feb 2026 5:39:03pm
    Author:  coren

  ==============================================================================
*/

#include "AbstractVoice.h"

void    AbstractVoice::setCurrentActiveUnisonVoices(const int unisonVoice)
{
    this->currentActiveUnisonVoices = unisonVoice;
}

void    AbstractVoice::setDetuneParam(const float detune)
{
    this->detuneParam = detune;
}


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
    const double sr = getSampleRate();

    if (sr <= 0.0)
    {
        // avoid division by zero
        for (int i = 0; i < MAX_UNISON; ++i)
        {
            phases[i] = 0.0f;
            phaseIncrements[i] = 0.0f;
        }
        return;
    }

    if (currentActiveUnisonVoices <= 1)
    {
        phases[0] = 0.0f;
        phaseIncrements[0] = static_cast<float> (frequency / sr);
        // clear remaining voices
        for (int i = 1; i < MAX_UNISON; ++i)
        {
            phases[i] = 0.0f;
            phaseIncrements[i] = 0.0f;
        }
    }

    else
    {
        for (int i = 0; i < currentActiveUnisonVoices; ++i)
        {
            // spread detune symmetrically across voices
            float detuneFactor = (static_cast<float> (i) / static_cast<float> (currentActiveUnisonVoices - 1)) * 2.0f - 1.0f;
            float currentFreq = frequency + (detuneFactor * detuneParam);

            // give each unison voice a slightly different start phase to reduce clicks from strict phase alignment
            phases[i] = juce::Random::getSystemRandom().nextFloat();
            phaseIncrements[i] = static_cast<float> (currentFreq / sr);
        }

        // clear any unused slots
        for (int i = currentActiveUnisonVoices; i < MAX_UNISON; ++i)
        {
            phases[i] = 0.0f;
            phaseIncrements[i] = 0.0f;
        }
    }

    // start a short attack to avoid clicks for unison; for single-voice, no attack so base wave is immediate
    attackSamplesRemaining = (currentActiveUnisonVoices > 1) ? attackLengthSamples : 0;
}