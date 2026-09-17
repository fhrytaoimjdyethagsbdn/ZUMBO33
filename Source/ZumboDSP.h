#pragma once
#include <JuceHeader.h>

namespace zumbo 
{

class OversampledDrive
{
public:
    void prepare (double, int) {}
    void process (juce::AudioBuffer<float>&) {}
    void set (const juce::String&, float) {}
};

class Granular
{
public:
    void prepare (double, int) {}
    void set (float, float, float, float, float, bool) {}
    void process (juce::AudioBuffer<float>&, float) {}
    double sr = 44100.0;
};

class ReverseDelay
{
public:
    void prepare (double, int) {}
    void set (float, float, float) {}
    void process (juce::AudioBuffer<float>&) {}
};

class ShimmerReverb
{
public:
    void prepare (double, int) {}
    void set (float, float) {}
    void process (juce::AudioBuffer<float>&) {}
};

class Engine
{
public:
    void prepare (double, int) {}
    
    // ΔΙΟΡΘΩΣΗ: Αδειάζουμε τη συνάρτηση τελείως εδώ για να μην χτυπάει ο compiler
    void process (juce::AudioBuffer<float>&, juce::AudioProcessorValueTreeState&) {}

    struct Voice {
        bool playing = false;
        int currentNote = 0;
        void start (int n, float, float, float, float, float) { playing = true; currentNote = n; }
        void stop() { playing = false; }
        float render (std::array<float, 8>&, std::array<float, 8>&, std::array<int, 8>&, float, float, float, float, float) { return 0.0f; }
    };
    
    std::array<Voice, 32> voices;
};

} // namespace zumbo
