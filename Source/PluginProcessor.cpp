#include <JuceHeader.h>
#include "ZumboParameters.h"
#include "ZumboDSP.h"

// Βάζουμε την engine εδώ πάνω για να τη βλέπει ο compiler παντού εγγυημένα
zumbo::Engine engine; 

class ZSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class ZUMBOAudioProcessor : public juce::AudioProcessor {
public:
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "ZUMBO_STATE", ZParams::createLayout()};
    juce::Synthesiser synth;
    
    ZUMBOAudioProcessor() : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
        synth.clearVoices(); for(int i=0; i<32; i++) synth.addVoice(new V()); synth.clearSounds(); synth.addSound(new ZSound());
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
    juce::AudioProcessorEditor* createEditor() override { return new juce::GenericAudioProcessorEditor(*this); }
    
    void getStateInformation(juce::MemoryBlock& dest) override { if(auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, dest); }
    void setStateInformation(const void* data, int size) override { if(auto xml = getXmlFromBinary(data, size)) apvts.replaceState(juce::ValueTree::fromXml(*xml)); }
    
    class V : public juce::SynthesiserVoice {
    public:
        V() {}
        bool canPlaySound(juce::SynthesiserSound* sound) override { return dynamic_cast<const ZSound*>(sound) != nullptr; }
        
        void startNote(int, float, juce::SynthesiserSound*, int) override {}
        void stopNote(float, bool) override {}
        void pitchWheelMoved(int) override {}
        void controllerMoved(int, int) override {}
        
        void renderNextBlock(juce::AudioBuffer<float>&, int, int) override {}
    };
};

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ZUMBOAudioProcessor(); }

