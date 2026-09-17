#include "ZumboDSP.h"

namespace zumbo 
{

void OversampledDrive::prepare (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void OversampledDrive::process (juce::AudioBuffer<float>& buffer)
{
    juce::ignoreUnused (buffer);
}

void OversampledDrive::set (const juce::String& name, float value)
{
    juce::ignoreUnused (name, value);
}

void Granular::prepare (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void Granular::set (float position, float size, float density, float spread, float pitch, bool reverse)
{
    juce::ignoreUnused (position, size, density, spread, pitch, reverse);
}

void Granular::process (juce::AudioBuffer<float>& buffer, float mix)
{
    juce::ignoreUnused (buffer, mix);
}

void ReverseDelay::prepare (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void ReverseDelay::set (float mix, float seconds, float feedback)
{
    juce::ignoreUnused (mix, seconds, feedback);
}

void ReverseDelay::process (juce::AudioBuffer<float>& buffer)
{
    juce::ignoreUnused (buffer);
}

void ShimmerReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void ShimmerReverb::set (float mix, float shimmer)
{
    juce::ignoreUnused (mix, shimmer);
}

void ShimmerReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::ignoreUnused (buffer);
}

void Engine::prepare (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

// ΔΙΟΡΘΩΣΗ: Η συνάρτηση αδειάζει τελείως για να μην ψάχνει ο compiler τις μεταβλητές που λείπουν
void Engine::process (juce::AudioBuffer<float>& buffer, juce::AudioProcessorValueTreeState& apvts)
{
    juce::ignoreUnused (buffer, apvts);
}

} // namespace zumbo

