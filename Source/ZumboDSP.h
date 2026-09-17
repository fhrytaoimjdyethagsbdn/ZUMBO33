#pragma once
#include <JuceHeader.h>
#include <array>
#include <vector>
#include <random>

namespace zumbo {

class PolyBLEPOsc {
 double sr=44100; float ph=0,inc=0; int wave=1;
 static float blep(float t,float dt) noexcept;
public:
 void prepare(double s){sr=s;reset();} void reset(){ph=0;}
 void setWave(int w){wave=w;} void setFrequency(float hz){inc=juce::jlimit(.000001f,.49f,hz/(float)sr);}
 float process() noexcept;
};

class OversampledDrive {
 juce::dsp::Oversampling<float> os{2,2,juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,true};
 float amount=0;
public:
 void prepare(double sr,int block); void reset(); void setAmount(float a){amount=a;}
 void process(juce::AudioBuffer<float>& b);
};

class SVF {
 double sr=44100; float ic1=0,ic2=0,g=0,k=1,drive=0; int mode=0;
 float one(float x) noexcept;
public:
 void prepare(double s){sr=s;reset();} void reset(){ic1=ic2=0;}
 void set(float cutoff,float res,int m,float d);
 float process(float x) noexcept;
};

class Granular {
 struct Grain{double pos=0,age=0,len=1,inc=1,pan=0;bool on=false;};
 juce::AudioBuffer<float> sample;
 std::array<Grain,256> grains{};
 std::mt19937 rng{0xBADC0DE}; std::uniform_real_distribution<float> rnd{0,1};
 double sr=44100,spawn=0; float pos=.5,size=.08,density=.3,spread=.5,pitch=0;bool rev=false;
 void newGrain();
public:
 void prepare(double s); void clear(); void load(const juce::AudioBuffer<float>& b);
 bool loaded()const{return sample.getNumSamples()>0;}
 void set(float p,float sz,float d,float sp,float pt,bool r){pos=p;size=sz;density=d;spread=sp;pitch=pt;rev=r;}
 void process(juce::AudioBuffer<float>& out,float mix);
};

class ReverseDelay {
 std::vector<float> l,r; int w=0; double sr=44100; float mix=0,fb=.3;int delay=16000;
public:
 void prepare(double s,int maxDelaySamples=192000);void reset();
 void set(float m,float seconds,float feedback);void process(juce::AudioBuffer<float>&);
};

class ShimmerReverb {
 juce::dsp::Reverb rv; juce::dsp::Reverb::Parameters p; float mix=0,sh=0;
 juce::dsp::Chorus<float> chorus;
public:
 void prepare(double sr,int block);void reset();void set(float m,float shimmer);void process(juce::AudioBuffer<float>&);
};

class Voice {
public:
 bool active=false;int note=60;float velocity=0;double sr=44100;
 std::array<PolyBLEPOsc,8> osc;SVF filter;juce::ADSR env;juce::ADSR::Parameters ep;
 void prepare(double s);void start(int n,float v,float a,float d,float sus,float rel);void stop();
 float render(const std::array<float,8>& lev,const std::array<float,8>& tune,const std::array<int,8>& wave,float cutoff,float res,int mode,float drive,float master);
};

class Engine {
public:
 std::array<Voice,32> voices;Granular granular;OversampledDrive nonlinear;ReverseDelay reverseDelay;ShimmerReverb reverb;
 void prepare(double sr,int block);void reset();
};

}
