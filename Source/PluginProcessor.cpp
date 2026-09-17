#include <JuceHeader.h>
#include "ZumboParameters.h"
#include "ZumboDSP.h"

class ZSound:public juce::SynthesiserSound{public:bool appliesToNote(int)override{return true;}bool appliesToChannel(int)override{return true;}};

class ZumboProcessor:public juce::AudioProcessor{
public:
 juce::AudioProcessorValueTreeState apvts{*this,nullptr,"ZUMBO_STATE",ZParams::createLayout()};
 zumbo::Engine engine;juce::Synthesiser synth;
 ZumboProcessor():AudioProcessor(BusesProperties().withOutput("Output",juce::AudioChannelSet::stereo(),true)){
  for(int i=0;i<32;i++)synth.addVoice(new V(*this));synth.addSound(new ZSound());
 }
 ~ZumboProcessor()override=default;const juce::String getName()const override{return"ZUMBO";}
 bool acceptsMidi()const override{return true;}bool producesMidi()const override{return false;}bool isMidiEffect()const override{return false;}
 double getTailLengthSeconds()const override{return 12;}int getNumPrograms()override{return 1;}int getCurrentProgram()override{return 0;}void setCurrentProgram(int)override{}const juce::String getProgramName(int)override{return"INIT";}void changeProgramName(int,const juce::String&)override{}
 void prepareToPlay(double sr,int block)override{engine.prepare(sr,block);synth.setCurrentPlaybackSampleRate(sr);}
 void releaseResources()override{}bool isBusesLayoutSupported(const BusesLayout&l)const override{return l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo();}
 float v(const juce::String&id)const{auto*q=apvts.getRawParameterValue(id);return q?q->load():0;}
 void processBlock(juce::AudioBuffer<float>&b,juce::MidiBuffer&m)override{
  juce::ScopedNoDenormals nd;b.clear();synth.renderNextBlock(b,m,0,b.getNumSamples());
  engine.nonlinear.setAmount(v("drive"));engine.nonlinear.process(b);
  engine.granular.set(v("grain_position"),v("grain_size"),v("grain_density"),v("grain_spread"),v("grain_pitch"),v("grain_reverse")>.5f);
  engine.granular.process(b,v("grain_mix"));
  engine.reverseDelay.set(v("reverse_mix"),v("delay_time"),v("delay_feedback"));engine.reverseDelay.process(b);
  engine.reverb.set(v("reverb_mix"),v("shimmer"));engine.reverb.process(b);
 }
 bool hasEditor()const override{return true;}juce::AudioProcessorEditor*createEditor()override;
 void getStateInformation(juce::MemoryBlock&d)override{if(auto x=apvts.copyState().createXml())copyXmlToBinary(*x,d);}
 void setStateInformation(const void*d,int s)override{if(auto x=getXmlFromBinary(d,s))apvts.replaceState(juce::ValueTree::fromXml(*x));}
 class V:public juce::SynthesiserVoice{
  ZumboProcessor&p;int idx=-1;
 public:V(ZumboProcessor&x):p(x){}bool canPlaySound(juce::SynthesiserSound*s)override{return dynamic_cast<ZSound*>(s)!=nullptr;}
  void startNote(int n,float vel,juce::SynthesiserSound*,int)override{auto&v=p.engine.voices[(size_t)(idx<0?0:idx)];v.start(n,vel,p.v("attack"),p.v("decay"),p.v("sustain"),p.v("release"));}
  void stopNote(float,bool tail)override{for(auto&v:p.engine.voices)if(v.active)v.stop();if(!tail)clearCurrentNote();}
  void pitchWheelMoved(int)override{}void controllerMoved(int,int)override{}
  void renderNextBlock(juce::AudioBuffer<float>&b,const juce::MidiBuffer&,int start,int n)override{
   auto&v=p.engine.voices[(size_t)(idx<0?0:idx)];std::array<float,8>l,t;std::array<int,8>w;
   for(int i=0;i<8;i++){auto s=juce::String(i+1);l[i]=p.v("osc"+s+"_level");t[i]=p.v("osc"+s+"_tune");w[i]=(int)p.v("osc"+s+"_wave");}
   for(int i=0;i<n;i++){float s=v.render(l,t,w,p.v("filter_cutoff"),p.v("filter_reso"),(int)p.v("filter_mode"),p.v("filter_drive"),p.v("master"));b.addSample(0,start+i,s);b.addSample(1,start+i,s);}
   if(!v.active)clearCurrentNote();
  }
 };
};
#include "PluginEditor.cpp"
juce::AudioProcessor*JUCE_CALLTYPE createPluginFilter(){return new ZumboProcessor();}
