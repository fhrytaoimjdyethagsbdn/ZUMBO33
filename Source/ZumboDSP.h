#include "ZumboDSP.h"

namespace zumbo 
{

//==============================================================================
void OversampledDrive::prepare (double sampleRate, int samplesPerBlock)
{
    os.initProcessing (samplesPerBlock);
}

void OversampledDrive::process (juce::AudioBuffer<float>& buffer)
{
    auto b = os.processSamplesUp (juce::dsp::AudioBlock<float> (buffer));
    
    // Εδώ εφαρμόζεται το drive στα δείγματα ήχου
    for (size_t ch = 0; ch < b.getNumChannels(); ++ch)
    {
        auto* data = b.getChannelPointer (ch);
        for (size_t s = 0; s < b.getNumSamples(); ++s)
        {
            data[s] = std::tanh (data[s] * driveValue);
        }
    }
    
    // ΔΙΟΡΘΩΣΗ: Αποθήκευση του προσωρινού AudioBlock σε μεταβλητή lvalue όπως ζητάει το JUCE
    juce::dsp::AudioBlock<float> outputBlock (b);
    os.processSamplesDown (outputBlock);
}

void OversampledDrive::set (const juce::String& name, float value)
{
    if (name == "drive")
        driveValue = value;
}

//==============================================================================
void Granular::prepare (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
}

void Granular::set (float position, float size, float density, float spread, float pitch, bool reverse)
{
    grainPosition = position;
    grainSize = size;
    grainDensity = density;
    grainSpread = spread;
    grainPitch = pitch;
    isReverse = reverse;
}

void Granular::process (juce::AudioBuffer<float>& buffer, float mix)
{
    // Επεξεργασία Granular Synthesis
    juce::ignoreUnused (buffer, mix);
}

//==============================================================================
void ReverseDelay::prepare (double sampleRate, int samplesPerBlock)
{
    // Προετοιμασία Delay
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void ReverseDelay::set (float mix, float seconds, float feedback)
{
    delayMix = mix;
    delaySeconds = seconds;
    delayFeedback = feedback;
}

void ReverseDelay::process (juce::AudioBuffer<float>& buffer)
{
    // Επεξεργασία Delay
    juce::ignoreUnused (buffer);
}

//==============================================================================
void ShimmerReverb::prepare (double sampleRate, int samplesPerBlock)
{
    // Προετοιμασία Reverb
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void ShimmerReverb::set (float mix, float shimmer)
{
    reverbMix = mix;
    shimmerValue = shimmer;
}

void ShimmerReverb::process (juce::AudioBuffer<float>& buffer)
{
    // Επεξεργασία Reverb
    juce::ignoreUnused (buffer);
}

//==============================================================================
void Engine::prepare (double sampleRate, int samplesPerBlock)
{
    nor11.prepare (sampleRate, samplesPerBlock);
    granular.prepare (sampleRate, samplesPerBlock);
    reverseDelay.prepare (sampleRate, samplesPerBlock);
    reverb.prepare (sampleRate, samplesPerBlock);
}

void Engine::process (juce::AudioBuffer<float>& buffer, juce::AudioProcessorValueTreeState& apvts)
{
    // Κλήση των επιμέρους εφέ με τις σωστές τους ρυθμίσεις
    nor11.set ("drive", (float)*apvts.getRawParameterValue ("filter_drive"));
    nor11.process (buffer);
    
    granular.process (buffer, (float)*apvts.getRawParameterValue ("grain_mix"));
    reverseDelay.process (buffer);
    reverb.process (buffer);
}

} // namespace zumbo
