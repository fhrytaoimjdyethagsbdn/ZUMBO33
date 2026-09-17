#include <JuceHeader.h>
#include "ZumboParameters.h"
#include "ZumboDSP.h"

class ZSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class ZUMBOAudioProcessor : public juce::AudioProcessor {
public:
    // Εδώ βάλαμε το ZParams::createLayout() που βρήκαμε στο αρχείο σου
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "ZUMBO_STATE", ZParams::createLayout()};
    zumbo::engine engine; juce::Synthesiser synth;
    
    ZUMBOAudioProcessor() : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
        synth.clearVoices(); for(int i=0; i<32; i++) synth.addVoice(new V(*this)); synth.clearSounds(); synth.addSound(new ZSound());
    }
    ~ZUMBOAudioProcessor() override {}
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
        juce::ScopedNoDenormals noDenormals; 
        synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());
        engine.process(buffer, apvts);
    }
    
    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    void getStateInformation(juce::MemoryBlock& dest) override { if(auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, dest); }
    void setStateInformation(const void* data, int size) override { if(auto xml = getXmlFromBinary(data, size)) apvts.replaceState(juce::ValueTree::fromXml(*xml)); }
    
    class V : public juce::SynthesiserVoice {
    public:
        ZUMBOAudioProcessor& pix; int idx = 1;
        V(ZUMBOAudioProcessor& p) : pix(p) {}
        bool canPlaySound(juce::SynthesiserSound* sound) override { return dynamic_cast<const ZSound*>(sound) != nullptr; }
        
        void startNote(int n, float vel, juce::SynthesiserSound*, int) override { 
            for(int i=0; i<32; i++) {
                if(!pix.synth.getVoice(i)->isVoiceActive()) { 
                    idx = i; 
                    break; 
                } 
            }
            pix.engine.voices[idx].start(n, vel, *pix.apvts.getRawParameterValue("attack"), *pix.apvts.getRawParameterValue("decay"), *pix.apvts.getRawParameterValue("sustain"), *pix.apvts.getRawParameterValue("release")); 
        }
        
        void stopNote(float, bool) override { 
            for(int i=0; i<32; i++) {
                auto* v = pix.synth.getVoice(i);
                if(v->isVoiceActive() && v->getCurrentlyPlayingNote() == getCurrentlyPlayingNote()) {
                    pix.engine.voices[i].stop();
                }
            }
        }
        
        void pitchWheelMoved(int) override {}
        void controllerMoved(int, int) override {}
        
        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
            std::array<float, 8> lev_arr;  lev_arr.fill(*pix.apvts.getRawParameterValue("lev"));
            std::array<float, 8> tune_arr; tune_arr.fill(*pix.apvts.getRawParameterValue("tune"));
            std::array<int, 8> wave_arr;   wave_arr.fill(int(*pix.apvts.getRawParameterValue("wave")));

            for (int i = 0; i < 32; i++) {
                if (pix.synth.getVoice(i)->isVoiceActive()) {
                    float sampleOut = pix.engine.voices[i].render(
                        lev_arr,
                        tune_arr,
                        wave_arr,
                        *pix.apvts.getRawParameterValue("filter_cutoff"), // Διορθώθηκε σε filter_cutoff βάσει του ZumboParameters.h
                        *pix.apvts.getRawParameterValue("filter_reso"),
                        *pix.apvts.getRawParameterValue("filter_mode"),
                        *pix.apvts.getRawParameterValue("filter_drive"),
                        *pix.apvts.getRawParameterValue("master")
                    );

                    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
                        auto* channelData = outputBuffer.getWritePointer(channel, startSample);
                        for (int s = 0; s < numSamples; ++s) {
                            channelData[s] += sampleOut;
                        }
                    }
                }
            }
        }
    };
};

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ZUMBOAudioProcessor(); }
