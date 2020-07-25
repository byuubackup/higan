#include <ws/ws.hpp>

namespace higan::WonderSwan {

APU apu;
#include "io.cpp"
#include "dma.cpp"
#include "channel1.cpp"
#include "channel2.cpp"
#include "channel3.cpp"
#include "channel4.cpp"
#include "channel5.cpp"
#include "serialization.cpp"

auto APU::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "APU");
  from = Node::scan(parent = node, from);

  stream = Node::append<Node::Stream>(parent, from, "Stream");
  stream->setChannels(2);
  stream->setFrequency(3'072'000);
  stream->addHighPassFilter(20.0, 1);
}

auto APU::unload() -> void {
  node = {};
  stream = {};
}

auto APU::main() -> void {
  dma.run();
  channel1.run();
  channel2.run();
  channel3.run();
  channel4.run();
  channel5.run();
  dacRun();
  if(++s.sweepClock == 0) channel3.sweep();
  step(1);
}

auto APU::sample(uint channel, uint5 index) -> uint4 {
  uint8 data = iram.read((r.waveBase << 6) + (--channel << 4) + (index >> 1));
  if(index.bit(0) == 0) return data.bit(0,3);
  if(index.bit(0) == 1) return data.bit(4,7);
  unreachable;
}

auto APU::dacRun() -> void {
  int left = 0;
  if(channel1.r.enable) left += channel1.o.left;
  if(channel2.r.enable) left += channel2.o.left;
  if(channel3.r.enable) left += channel3.o.left;
  if(channel4.r.enable) left += channel4.o.left;
  if(channel5.r.enable) left += channel5.o.left * r.headphonesConnected;
  left = sclamp<16>(left << 5);

  int right = 0;
  if(channel1.r.enable) right += channel1.o.right;
  if(channel2.r.enable) right += channel2.o.right;
  if(channel3.r.enable) right += channel3.o.right;
  if(channel4.r.enable) right += channel4.o.right;
  if(channel5.r.enable) right += channel5.o.right * r.headphonesConnected;
  right = sclamp<16>(right << 5);

  if(!r.headphonesConnected) {
    left = right = (left + right) / 2 >> 3 - r.speakerShift;  //monaural output
    if(!r.speakerEnable) {
      left = 0;
      right = 0;
    }
  } else {
    if(!r.headphonesEnable) {
      left = 0;
      right = 0;
    }
  }

  //ASWAN has three volume steps (0%, 50%, 100%); SPHINX and SPHINX2 have four (0%, 33%, 66%, 100%)
  double amplitude = 1.0 / (SoC::ASWAN() ? 2.0 : 3.0) * r.masterVolume;
  stream->sample(left / 32768.0 * amplitude, right / 32768.0 * amplitude);
}

auto APU::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto APU::power() -> void {
  Thread::create(3'072'000, {&APU::main, this});

  bus.map(this, 0x004a, 0x004c);
  bus.map(this, 0x004e, 0x0050);
  bus.map(this, 0x0052);
  bus.map(this, 0x006a, 0x006b);
  bus.map(this, 0x0080, 0x0095);
  bus.map(this, 0x009e);

  s.sweepClock = 0;
  r.waveBase = 0;
  r.speakerEnable = 0;
  r.speakerShift = 0;
  r.headphonesEnable = 0;
  r.headphonesConnected = system.headphones->value();
  r.masterVolume = SoC::ASWAN() ? 2 : 3;

  dma.s.clock = 0;
  dma.s.source = 0;
  dma.s.length = 0;
  dma.r.source = 0;
  dma.r.length = 0;
  dma.r.rate = 0;
  dma.r.unknown = 0;
  dma.r.loop = 0;
  dma.r.target = 0;
  dma.r.direction = 0;
  dma.r.enable = 0;

  channel1.o.left = 0;
  channel1.o.right = 0;
  channel1.s.period = 0;
  channel1.s.sampleOffset = 0;
  channel1.r.pitch = 0;
  channel1.r.volumeLeft = 0;
  channel1.r.volumeRight = 0;
  channel1.r.enable = 0;

  channel2.o.left = 0;
  channel2.o.right = 0;
  channel2.s.period = 0;
  channel2.s.sampleOffset = 0;
  channel2.r.pitch = 0;
  channel2.r.volumeLeft = 0;
  channel2.r.volumeRight = 0;
  channel2.r.enable = 0;
  channel2.r.voice = 0;
  channel2.r.voiceEnableLeft = 0;
  channel2.r.voiceEnableRight = 0;

  channel3.o.left = 0;
  channel3.o.right = 0;
  channel3.s.period = 0;
  channel3.s.sampleOffset = 0;
  channel3.s.sweepCounter = 0;
  channel3.r.pitch = 0;
  channel3.r.volumeLeft = 0;
  channel3.r.volumeRight = 0;
  channel3.r.sweepValue = 0;
  channel3.r.sweepTime = 0;
  channel3.r.enable = 0;
  channel3.r.sweep = 0;

  channel4.o.left = 0;
  channel4.o.right = 0;
  channel4.s.period = 0;
  channel4.s.sampleOffset = 0;
  channel4.s.noiseOutput = 0;
  channel4.s.noiseLFSR = 0;
  channel4.r.pitch = 0;
  channel4.r.volumeLeft = 0;
  channel4.r.volumeRight = 0;
  channel4.r.noiseMode = 0;
  channel4.r.noiseReset = 0;
  channel4.r.noiseUpdate = 0;
  channel4.r.enable = 0;
  channel4.r.noise = 0;

  channel5.o.left = 0;
  channel5.o.right = 0;
  channel5.s.clock = 0;
  channel5.s.data = 0;
  channel5.r.volume = 0;
  channel5.r.scale = 0;
  channel5.r.speed = 0;
  channel5.r.enable = 0;
  channel5.r.unknown = 0;
  channel5.r.leftEnable = 0;
  channel5.r.rightEnable = 0;
}

}
