#include <JuceHeader.h>
#include "ZumboParameters.h"
#include "ZumboDSP.h"

class ZSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class ZumboProcessor : public juce::AudioProcessor {
public:
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "ZUMBO_STATE", Params::createLayout()};
    zumbo::Engine engine; juce::Synthesiser synth;
    ZumboProcessor() : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
        synth.clearVoices(); for(int i=0; i<32; i++) synth.addVoice(new V(*this)); synth.clearSounds(); synth.addSound(new ZSound());
    }
    ~ZumboProcessor() override {}
    const juce::String getName() const override { return "ZUMBO"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 12; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return ""; }
    void changeProgramName(int, const juce::String&) override {}
    void prepareToPlay(double sr, int block) override { engine.prepare(sr, block); synth.setCurrentPlaybackSampleRate(sr); }
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override { return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo(); }
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override {
        juce::ScopedNoDenormals noDenormals; synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());
        engine.nor11.set("drive", (float)*apvts.getRawParameterValue("fdr"));
        engine.granular._sr = getSampleRate();
        engine.granular.set("grain_size", *apvts.getRawParameterValue("grain_size"), *apvts.getRawParameterValue("grain_spread"), *apvts.getRawParameterValue("grain_pd"));
        engine.granular.process(buffer, *apvts.getRawParameterValue("grain_mix"));
        engine.reverseDelay.set("reverse_mix", *apvts.getRawParameterValue("delay_mix"), *apvts.getRawParameterValue("delay_feedback")); engine.reverseDelay.process(buffer);
        engine.reverb.set("reverb_mix", *apvts.getRawParameterValue("reverb_mix")); engine.reverb.process(buffer);
    }
    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    void getStateInformation(juce::MemoryBlock& dest) override { if(auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, dest); }
    void setStateInformation(const void* data, int size) override { if(auto xml = getXmlFromBinary(data, size)) apvts.replaceState(juce::ValueTree::fromXml(*xml)); }
    class V : public juce::SynthesiserVoice {
    public:
        ZumboProcessor& pix; int idx = 1;
        V(ZumboProcessor& p) : pix(p) {}
        bool canPlaySound(juce::SynthesiserSound* sound) override { return dynamic_cast<const ZSound*>(sound) != nullptr; }
        void startNote(int n, float vel, juce::SynthesiserSound*, int) override { for(int i=0; i<32; i++) if(!pix.engine.voices[(idx+i)%32].isPlaying) { idx=(idx+i)%32; break; } pix.engine.voices[idx].start(n, vel, *pix.apvts.getRawParameterValue("attack"), *pix.apvts.getRawParameterValue("decay"), *pix.apvts.getRawParameterValue("sustain"), *pix.apvts.getRawParameterValue("release")); }
        void stopNote(float, bool) override { for(auto&v:pix.engine.voices) if(v.isPlaying&&v.currentNote==getCurrentlyPlayingNote()) v.stop(); }
        void pitchWheelMoved(int) override {}
        void controllerMoved(int, int) override {}
        
        // Αυτή είναι η σωστή και απλή μορφή της συνάρτησης όπως την είχες γράψει αρχικά!
        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
            juce::AudioBuffer<float> sub(outputBuffer.getArrayOfWritePointers(), outputBuffer.getNumChannels(), startSample, numSamples);
            for(int i=0; i<32; i++) if(pix.engine.voices[(idx+i)%32].isPlaying) { pix.engine.voices[(idx+i)%32].render(sub); }
        }
    };
};

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ZumboProcessor(); }
