#include "ZumboDSP.h"
#include <iostream>
#include <cmath>
int main(){
 using namespace zumbo;
 PolyBLEPOsc o;o.prepare(48000);o.setFrequency(440);o.setWave(1);
 double sum=0,maxv=0;for(int i=0;i<48000;i++){float x=o.process();if(!std::isfinite(x))return 2;sum+=x*x;maxv=std::max(maxv,(double)std::abs(x));}
 if(maxv>2.0)return 3;
 SVF f;f.prepare(48000);f.set(1000,.2,0,.1);for(int i=0;i<48000;i++)if(!std::isfinite(f.process(std::sin(i*.1f))))return 4;
 Granular g;g.prepare(48000);juce::AudioBuffer<float> src(1,48000);for(int i=0;i<48000;i++)src.setSample(0,i,std::sin(juce::MathConstants<float>::twoPi*440*i/48000));g.load(src);g.set(.5,.05,.8,.5,0,false);juce::AudioBuffer<float> out(2,512);g.process(out,1);for(int c=0;c<2;c++)for(int i=0;i<512;i++)if(!std::isfinite(out.getSample(c,i)))return 5;
 std::cout<<"ZUMBO DSP TESTS PASSED\n";return 0;
}
