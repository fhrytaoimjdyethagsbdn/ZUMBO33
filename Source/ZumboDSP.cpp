#include "ZumboDSP.h"
namespace zumbo {
float PolyBLEPOsc::blep(float t,float dt)noexcept{
 if(t<dt){t/=dt;return t+t-t*t-1;} if(t>1-dt){t=(t-1)/dt;return t*t+t+t+1;} return 0;
}
float PolyBLEPOsc::process()noexcept{
 float y=0;
 if(wave==0)y=std::sin(juce::MathConstants<float>::twoPi*ph);
 else if(wave==1){y=2*ph-1;y-=blep(ph,inc);}
 else if(wave==2){y=ph<.5?1:-1;y+=blep(ph,inc);float t=ph+.5f;if(t>=1)t-=1;y-=blep(t,inc);}
 else y=1-4*std::abs(ph-.5f);
 ph+=inc;ph-=std::floor(ph);return y;
}
void OversampledDrive::prepare(double sr,int block){os.reset();os.initProcessing(sr);ignoreUnused(block);}
void OversampledDrive::reset(){os.reset();}
void OversampledDrive::process(juce::AudioBuffer<float>& b){
 if(amount<=.0001f)return;auto up=os.processSamplesUp(juce::dsp::AudioBlock<float>(b));
 for(size_t c=0;c<up.getNumChannels();++c)for(size_t i=0;i<up.getNumSamples();++i)up.getChannelPointer(c)[i]=std::tanh(up.getChannelPointer(c)[i]*(1+amount*10));
 os.processSamplesDown(juce::dsp::AudioBlock<float>(b));
}
void SVF::set(float cutoff,float res,int m,float d){mode=m;drive=d;float f=juce::jlimit(20.f,(float)sr*.45f,cutoff);g=std::tan(juce::MathConstants<float>::pi*f/(float)sr);k=2-1.98f*juce::jlimit(0.f,.99f,res);}
float SVF::one(float x)noexcept{ x=std::tanh(x*(1+drive*7));float a=1/(1+g*(g+k));float v3=x-ic2;float v1=a*ic1+g*a*v3;float v2=ic2+g*v1;ic1=2*v1-ic1;ic2=2*v2-ic2;float low=v2,high=x-k*v1-v2,band=v1,notch=low+high;return mode==0?low:mode==1?low:mode==2?high:mode==3?high:mode==4?band:mode==5?band:mode==6?notch:band;}
float SVF::process(float x)noexcept{return one(x);}
void Granular::prepare(double s){sr=s;clear();}
void Granular::clear(){sample.setSize(0,0);for(auto&g:grains)g.on=false;}
void Granular::load(const juce::AudioBuffer<float>&b){sample.makeCopyOf(b);for(auto&g:grains)g.on=false;}
void Granular::newGrain(){if(!loaded())return;for(auto&g:grains)if(!g.on){double len=juce::jmax(32.0,size*sr);double center=pos*(sample.getNumSamples()-1);double jitter=(rnd(rng)-.5)*spread*len*4;g.pos=juce::jlimit(0.,(double)sample.getNumSamples()-1,center+jitter);g.len=len;g.age=0;g.inc=std::pow(2.,pitch/12.)*(rev?-1:1);g.pan=rnd(rng)*2-1;g.on=true;return;}}
void Granular::process(juce::AudioBuffer<float>&out,float mix){if(!loaded()||mix<=.0001f)return;for(int i=0;i<out.getNumSamples();++i){spawn-=density*.5;if(spawn<=0){newGrain();spawn=1;}for(auto&g:grains)if(g.on){int n=juce::jlimit(0,sample.getNumSamples()-1,(int)g.pos);float ph=g.age/g.len;float e=.5-.5*std::cos(juce::MathConstants<float>::twoPi*ph);float s=sample.getSample(0,n)*e*mix*.25f;float L=s*std::sqrt(.5*(1-g.pan)),R=s*std::sqrt(.5*(1+g.pan));out.addSample(0,i,L);if(out.getNumChannels()>1)out.addSample(1,i,R);g.pos+=g.inc;g.age++;if(g.age>=g.len||g.pos<0||g.pos>=sample.getNumSamples())g.on=false;}}}
void ReverseDelay::prepare(double s,int max){sr=s;l.assign(max,0);r.assign(max,0);reset();}
void ReverseDelay::reset(){std::fill(l.begin(),l.end(),0);std::fill(r.begin(),r.end(),0);w=0;}
void ReverseDelay::set(float m,float sec,float f){mix=m;fb=f;delay=juce::jlimit(1,(int)l.size()-1,(int)(sec*sr));}
void ReverseDelay::process(juce::AudioBuffer<float>&b){if(mix<=0)return;for(int i=0;i<b.getNumSamples();++i){int rd=(w-delay+(int)l.size())%(int)l.size();float dl=l[rd],dr=r[rd];float inL=b.getSample(0,i),inR=b.getSample(1,i);l[w]=inL+dl*fb;r[w]=inR+dr*fb;b.setSample(0,i,inL+dl*mix);b.setSample(1,i,inR+dr*mix);w=(w+1)%l.size();}}
void ShimmerReverb::prepare(double sr,int block){juce::dsp::ProcessSpec sp{sr,(juce::uint32)block,2};rv.prepare(sp);chorus.prepare(sp);chorus.setCentreDelay(18);chorus.setDepth(.7);chorus.setRate(.15);}
void ShimmerReverb::reset(){rv.reset();chorus.reset();}
void ShimmerReverb::set(float m,float s){mix=m;sh=s;p.roomSize=.86;p.damping=.25;p.width=1;p.wetLevel=m;p.dryLevel=1-m;rv.setParameters(p);}
void ShimmerReverb::process(juce::AudioBuffer<float>&b){juce::dsp::AudioBlock<float> bl(b);rv.process(juce::dsp::ProcessContextReplacing<float>(bl));if(sh>.01f)chorus.process(juce::dsp::ProcessContextReplacing<float>(bl));}
void Voice::prepare(double s){sr=s;filter.prepare(s);for(auto&o:osc)o.prepare(s);env.setSampleRate(s);}
void Voice::start(int n,float v,float a,float d,float sus,float rel){note=n;velocity=v;active=true;ep={a,d,sus,rel};env.setParameters(ep);env.reset();env.noteOn();for(auto&o:osc)o.reset();}
void Voice::stop(){env.noteOff();}
float Voice::render(const std::array<float,8>&lev,const std::array<float,8>&tune,const std::array<int,8>&wave,float cutoff,float res,int mode,float drive,float master){
 float hz=juce::MidiMessage::getMidiNoteInHertz(note),s=0;for(int i=0;i<8;i++){osc[i].setWave(wave[i]);osc[i].setFrequency(hz*std::pow(2.f,tune[i]/12));s+=osc[i].process()*lev[i];}filter.set(cutoff,res,mode,drive);s=filter.process(s)*.09f*velocity*env.getNextSample()*master;if(!env.isActive())active=false;return s;
}
void Engine::prepare(double sr,int block){for(auto&v:voices)v.prepare(sr);granular.prepare(sr);nonlinear.prepare(sr,block);reverseDelay.prepare(sr);reverb.prepare(sr,block);}
void Engine::reset(){for(auto&v:voices)v.active=false;granular.clear();nonlinear.reset();reverseDelay.reset();reverb.reset();}
}
