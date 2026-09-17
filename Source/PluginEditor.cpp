#include <JuceHeader.h>
#include "PluginProcessor.cpp" // Διαβάζει αυτόματα την κλάση ZUMBOAudioProcessor

class ZUMBOAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ZUMBOAudioProcessor& audioProcessor;

    ZUMBOAudioProcessorEditor (ZUMBOAudioProcessor& p)
        : AudioProcessorEditor (&p), audioProcessor (p)
    {
        setSize (400, 300);
    }

    ~ZUMBOAudioProcessorEditor() override {}

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
        g.setColour (juce::Colours::white);
        g.setFont (15.0f);
        g.drawFittedText ("ZUMBO Synthesizer", getLocalBounds(), juce::Justification::centred, 1);
    }

    void resized() override {}
};

// Αυτή η συνάρτηση συνδέει τον Processor με τον Editor
juce::AudioProcessorEditor* ZUMBOAudioProcessor::createEditor()
{
    return new ZUMBOAudioProcessorEditor (*this);
}
