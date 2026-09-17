#include <JuceHeader.h>
#include "ZumboLookAndFeel.h"

class ZumboEditor:public juce::AudioProcessorEditor{
 ZumboProcessor&p;ZumboLookAndFeel lf;juce::Label title,sub;std::array<juce::Slider,16> knobs;std::array<juce::Label,16> labs;
 juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> att;
 const char*ids[16]={"grain_mix","grain_density","grain_position","grain_size","filter_cutoff","filter_reso","filter_drive","drive","delay_mix","delay_time","delay_feedback","reverse_mix","reverb_mix","shimmer","width","master"};
 const char*names[16]={"GRAIN","DENSITY","POSITION","SIZE","CUTOFF","RESONANCE","FILTER DRIVE","DISTORT","ECHO","ECHO TIME","FEEDBACK","REVERSE","REVERB","SHIMMER","WIDTH","MASTER"};
public:
 ZumboEditor(ZumboProcessor&x):AudioProcessorEditor(&x),p(x){
  setLookAndFeel(&lf);setResizable(true,true);setSize(1500,920);
  title.setText("ZUMBO",juce::dontSendNotification);title.setFont(juce::FontOptions(58,juce::Font::bold));title.setColour(juce::Label::textColourId,juce::Colour(0xffefff50));addAndMakeVisible(title);
  sub.setText("INFINITE FOREST // GRANULAR • 8 OSC • MOD MATRIX • SHIMMER",juce::dontSendNotification);sub.setColour(juce::Label::textColourId,juce::Colour(0xffc66cff));addAndMakeVisible(sub);
  for(int i=0;i<16;i++){knobs[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);knobs[i].setTextBoxStyle(juce::Slider::TextBoxBelow,false,70,20);addAndMakeVisible(knobs[i]);labs[i].setText(names[i],juce::dontSendNotification);labs[i].setJustificationType(juce::Justification::centred);addAndMakeVisible(labs[i]);att.add(new juce::AudioProcessorValueTreeState::SliderAttachment(p.apvts,ids[i],knobs[i]));}
 }
 ~ZumboEditor()override{setLookAndFeel(nullptr);}
 void paint(juce::Graphics&g)override{auto b=getLocalBounds().toFloat();juce::ColourGradient q(juce::Colour(0xff05020a),0,0,juce::Colour(0xff23062f),b.getRight(),b.getBottom(),false);g.setGradientFill(q);g.fillAll();for(int i=0;i<20;i++){float x=40+i*75;float y=130+30*std::sin(i);g.setColour(juce::Colour(0xffbd39ff).withAlpha(.018f));g.fillEllipse(x-90,y-90,180,180);}g.setColour(juce::Colour(0xff321348));g.drawRoundedRectangle(b.reduced(14),25,1);auto v=b.reduced(38).withTrimmedTop(115).withTrimmedBottom(315);g.setColour(juce::Colour(0xff08040e));g.fillRoundedRectangle(v,25);juce::Path w;w.startNewSubPath(v.getX(),v.getCentreY());for(int x=0;x<v.getWidth();x++){float n=(float)x/v.getWidth();w.lineTo(v.getX()+x,v.getCentreY()+std::sin(n*77)*25+std::sin(n*193)*10);}g.setColour(juce::Colour(0xffc54cff));g.strokePath(w,juce::PathStrokeType(2));}
 void resized()override{auto r=getLocalBounds().reduced(32);title.setBounds(r.removeFromTop(62));sub.setBounds(r.removeFromTop(26));auto b=r.removeFromBottom(285);int cw=b.getWidth()/16;for(int i=0;i<16;i++){auto c=b.removeFromLeft(cw);labs[i].setBounds(c.removeFromBottom(25));knobs[i].setBounds(c.reduced(5));}}
};
juce::AudioProcessorEditor*ZumboProcessor::createEditor(){return new ZumboEditor(*this);}
