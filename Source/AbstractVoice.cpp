/*
  ==============================================================================

    AbstractVoice.cpp
    Created: 11 Feb 2026 5:39:03pm
    Author:  coren

  ==============================================================================
*/

#include "AbstractVoice.h"
#include <cmath>

AbstractVoice::AbstractVoice()
{
    adsr.setSampleRate(getSampleRate());
}

void            AbstractVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    adsr.setParameters(adsrParams);
}

void AbstractVoice::setCurrentActiveUnisonVoices(const int unisonVoice)
{
    currentActiveUnisonVoices = juce::jlimit(1, MAX_UNISON, unisonVoice);
}

void    AbstractVoice::updateDetuneParam(const float detune)
{
    if (detune == this->detuneParam)
        return;
    const double sr = getSampleRate();
    if (sr <= 0.0)
        return;
    this->detuneParam = detune;
    for (int i = 0; i < currentActiveUnisonVoices; ++i)
    {
        float detuneFactor = (currentActiveUnisonVoices > 1)
            ? (static_cast<float>(i) / static_cast<float>(currentActiveUnisonVoices - 1)) * 2.0f - 1.0f
            : 0.0f;

        // detuneParam is interpreted in cents now; detuneFactor in [-1,1]
        float cents = detuneFactor * detuneParam; // detuneParam in cents
        float freqMultiplier = std::pow (2.0f, cents / 1200.0f);
        float currentFreq = frequency * freqMultiplier;
        phaseIncrementTargets[i] = static_cast<float> (currentFreq / sr);
    }

    // clear unused targets
    for (int i = currentActiveUnisonVoices; i < MAX_UNISON; ++i)
        phaseIncrementTargets[i] = 0.0f;

    // compute smoothing alpha (per-sample) from tau and sample rate:
    // alpha = 1 - exp(-1 / (tau * sr))  -> used as the smoothing coefficient
    detuneSmoothingAlpha = 1.0f - std::exp(-1.0f / (detuneSmoothingTauSeconds * (float)sr));

    // a small attack when increasing unison can still help: start attack only if >1
    //attackSamplesRemaining = (currentActiveUnisonVoices > 1) ? attackLengthSamples : 0;
}


bool	AbstractVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<VoiceSound*> (sound) != nullptr;
}

void	AbstractVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)

        adsr.noteOff();
    else
    {
        adsr.reset();
        clearCurrentNote();
    }
}

void	AbstractVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    adsr.noteOn();
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    amplitude = velocity * 0.15f;
    const double sr = getSampleRate();
    if (sr <= 0.0)
    {
        for (int i = 0; i < MAX_UNISON; ++i)
        {
            phases[i] = 0.0f;
            phaseIncrements[i] = 0.0f;
            phaseIncrementTargets[i] = 0.0f;
        }
        return;
    }

    // initialize targets and current increments; keep phases randomized to reduce phase-alignment clicks
    for (int i = 0; i < currentActiveUnisonVoices; ++i)
    {
        float detuneFactor = (currentActiveUnisonVoices > 1)
            ? (static_cast<float>(i) / static_cast<float>(currentActiveUnisonVoices - 1)) * 2.0f - 1.0f
            : 0.0f;

        // detuneParam is interpreted in cents now; detuneFactor in [-1,1]
        float cents = detuneFactor * detuneParam; // detuneParam in cents
        float freqMultiplier = std::pow (2.0f, cents / 1200.0f);
        float currentFreq = frequency * freqMultiplier;
        phaseIncrementTargets[i] = static_cast<float> (currentFreq / sr);

        // set current increment equal to target so there's no abrupt jump at note-on
        phaseIncrements[i] = phaseIncrementTargets[i];

        // slightly randomize start phase for each voice to reduce phasing artifacts
        phases[i] = juce::Random::getSystemRandom().nextFloat();
    }

    // clear any unused slots
    for (int i = currentActiveUnisonVoices; i < MAX_UNISON; ++i)
    {
        phases[i] = 0.0f;
        phaseIncrements[i] = 0.0f;
        phaseIncrementTargets[i] = 0.0f;
    }

    detuneSmoothingAlpha = 1.0f - std::exp(-1.0f / (detuneSmoothingTauSeconds * (float)sr));

    // start a short attack to avoid clicks for unison; for single-voice, no attack so base wave is immediate
    attackSamplesRemaining = (currentActiveUnisonVoices > 1) ? attackLengthSamples : 0;
}