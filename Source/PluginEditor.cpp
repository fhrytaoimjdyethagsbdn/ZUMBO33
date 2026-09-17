#include <JuceHeader.h>

// Λέμε στον compiler ότι η κλάση ZUMBOAudioProcessor υπάρχει κάπου αλλού
class ZUMBOAudioProcessor; 

class ZUMBOAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    juce::AudioProcessor& audioProcessor;

    ZUMBOAudioProcessorEditor (juce::AudioProcessor& p)
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
