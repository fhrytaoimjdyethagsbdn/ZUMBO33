#pragma once
#include <JuceHeader.h>

namespace ZParams {
using P=juce::AudioProcessorValueTreeState::ParameterLayout;
inline P createLayout(){
 P p;
 auto f=[&](juce::String id,juce::String n,float lo,float hi,float d){
  p.add(std::make_unique<juce::AudioParameterFloat>(id,n,juce::NormalisableRange<float>(lo,hi,0.0001f),d));
 };
 for(int i=1;i<=8;i++){
  auto s=juce::String(i); f("osc"+s+"_level","OSC "+s+" Level",0,1,.65f);
  f("osc"+s+"_tune","OSC "+s+" Tune",-36,36,0); f("osc"+s+"_pan","OSC "+s+" Pan",-1,1,0);
  p.add(std::make_unique<juce::AudioParameterChoice>("osc"+s+"_wave","OSC "+s+" Wave",juce::StringArray{"Sine","Saw","Square","Triangle","Wavetable"},1));
 }
 f("master","Master",0,1,.8f); f("unison","Unison",1,8,2); f("detune","Detune",0,1,.12f);
 f("grain_mix","Granular Mix",0,1,0); f("grain_position","Grain Position",0,1,.5f);
 f("grain_size","Grain Size",0.005f,1,.08f); f("grain_density","Grain Density",0,1,.35f);
 f("grain_spread","Grain Spread",0,1,.5f); f("grain_pitch","Grain Pitch",-24,24,0);
 p.add(std::make_unique<juce::AudioParameterBool>("grain_reverse","Grain Reverse",false));
 p.add(std::make_unique<juce::AudioParameterChoice>("filter_mode","Filter Mode",
   juce::StringArray{"LP24","LP48","HP24","HP48","BP24","BP48","Notch","Comb"},0));
 f("filter_cutoff","Filter Cutoff",20,20000,1200); f("filter_reso","Filter Resonance",0,.98,.2); f("filter_drive","Filter Drive",0,1,.1);
 f("attack","Attack",.001,5,.005); f("decay","Decay",.005,5,.15); f("sustain","Sustain",0,1,.8); f("release","Release",.005,8,.3);
 for(int i=1;i<=4;i++){auto s=juce::String(i);f("lfo"+s+"_rate","LFO "+s+" Rate",.01,40,2);f("lfo"+s+"_amount","LFO "+s+" Amount",-1,1,0);}
 // 16 modulation slots: source, destination, bipolar amount.
 const juce::StringArray sources={"LFO1","LFO2","LFO3","LFO4","ENV","Velocity","ModWheel","Random","Aftertouch"};
 const juce::StringArray dests={"Cutoff","Resonance","GrainPos","GrainSize","GrainDensity","OscPitch","OscMix","Pan","Drive","Delay","Reverb","Width"};
 for(int i=1;i<=16;i++){auto s=juce::String(i);p.add(std::make_unique<juce::AudioParameterChoice>("mod"+s+"_src","Mod "+s+" Source",sources,0));p.add(std::make_unique<juce::AudioParameterChoice>("mod"+s+"_dst","Mod "+s+" Destination",dests,0));f("mod"+s+"_amt","Mod "+s+" Amount",-1,1,0);}
 f("drive","Drive",0,1,0); f("delay_mix","Echo Mix",0,1,.15); f("delay_time","Echo Time",.01,2,.375); f("delay_feedback","Echo Feedback",0,.97,.35); f("reverse_mix","Reverse Delay Mix",0,1,0); f("reverb_mix","Reverb Mix",0,1,.18);f("shimmer","Shimmer",0,1,0);f("width","Width",0,2,1);
 return p;
}}
