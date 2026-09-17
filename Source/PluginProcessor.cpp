#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZUMBOAudioProcessor::ZUMBOAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    synth.clearVoices();
    for (int i = 0; i < 32; i++)
    {
        synth.addVoice(new V(*this));
    }
    synth.clearSounds();
    synth.addSound(new ZSound());
}

ZUMBOAudioProcessor::~ZUMBOAudioProcessor()
{
}

//==============================================================================
const juce::String ZUMBOAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ZUMBOAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ZUMBOAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ZUMBOAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ZUMBOAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ZUMBOAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ZUMBOAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ZUMBOAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ZUMBOAudioProcessor::getProgramName (int index)
{
    return {};
}

void ZUMBOAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ZUMBOAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepare(sampleRate, samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void ZUMBOAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ZUMBOAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some hosts    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ZUMBOAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    engine.process(buffer, apvts);
}

//==============================================================================
bool ZUMBOAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not have an editor)
}

juce::AudioProcessorEditor* ZUMBOAudioProcessor::createEditor()
{
    return new ZUMBOAudioProcessorEditor (*this);
}

//==============================================================================
void ZUMBOAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void ZUMBOAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZUMBOAudioProcessor();
}
