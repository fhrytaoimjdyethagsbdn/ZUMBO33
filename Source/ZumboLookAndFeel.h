#pragma once
#include <JuceHeader.h>
class ZumboLookAndFeel:public juce::LookAndFeel_V4{
public:
 ZumboLookAndFeel(){setColour(juce::ResizableWindow::backgroundColourId,juce::Colour(0xff07020d));setColour(juce::Slider::thumbColourId,juce::Colour(0xffefff4e));setColour(juce::Slider::textBoxTextColourId,juce::Colour(0xffeadfff));setColour(juce::Slider::textBoxBackgroundColourId,juce::Colour(0xff100819));}
 void drawRotarySlider(juce::Graphics&g,int x,int y,int w,int h,float pos,float a0,float a1,juce::Slider&)override{auto r=juce::Rectangle<float>((float)x,(float)y,(float)w,(float)h).reduced(8);auto c=r.getCentre();float rad=juce::jmin(r.getWidth(),r.getHeight())*.5f;for(int i=5;i>=0;i--){g.setColour(juce::Colour(0xffbb36ff).withAlpha(.02f*(6-i)));g.fillEllipse(c.x-rad-i*3,c.y-rad-i*3,(rad+i*3)*2,(rad+i*3)*2);}g.setColour(juce::Colour(0xff150b21));g.fillEllipse(c.x-rad,c.y-rad,rad*2,rad*2);juce::Path p;p.addCentredArc(c.x,c.y,rad-4,rad-4,0,a0,a0+pos*(a1-a0),true);g.setColour(juce::Colour(0xffc94dff));g.strokePath(p,juce::PathStrokeType(4));}
};
