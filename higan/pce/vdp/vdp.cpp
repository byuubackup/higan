#if defined(PROFILE_PERFORMANCE)
#include "../vdp-performance/vdp.cpp"
#else
#include <pce/pce.hpp>

namespace higan::PCEngine {

VDP vdp;
#include "vce.cpp"
#include "vdc.cpp"
#include "vpc.cpp"
#include "irq.cpp"
#include "dma.cpp"
#include "background.cpp"
#include "sprite.cpp"
#include "color.cpp"
#include "serialization.cpp"

auto VDP::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "VDP");
  from = Node::scan(parent = node, from);

  screen = Node::append<Node::Screen>(parent, from, "Screen");
  screen->colors(1 << 10, {&VDP::color, this});
  screen->setSize(1024, 239);
  screen->setScale(0.25, 1.0);
  screen->setAspect(8.0, 7.0);
  from = Node::scan(parent = screen, from);
}

auto VDP::unload() -> void {
  node = {};
  screen = {};
}

auto VDP::main() -> void {
  vdc0.hsync(); if(Model::SuperGrafx())
  vdc1.hsync();

  if(io.vcounter == 0) {
    vdc0.vsync(); if(Model::SuperGrafx())
    vdc1.vsync();
  }

  auto output = buffer + 1365 * io.vcounter;

  while(io.hcounter <= 1360) {
    vdc0.hclock(); if(Model::SuperGrafx())
    vdc1.hclock();

    uint10 color;
    if(Model::PCEngine()) color = vdc0.bus();
    if(Model::SuperGrafx()) color = vpc.bus(io.hcounter);
    color = vce.io.grayscale << 9 | vce.cram.read(color);

    if(vce.clock() >= 2) *output++ = color;
    if(vce.clock() >= 2) *output++ = color;
    if(vce.clock() >= 3) *output++ = color;
    if(vce.clock() >= 4) *output++ = color;

    step(vce.clock());
  }

  step(1365 - io.hcounter);
  vdc0.vclock(); if(Model::SuperGrafx())
  vdc1.vclock();

  io.hcounter = 0;
  if(++io.vcounter >= 262 + vce.io.extraLine) {
    io.vcounter = 0;
    scheduler.exit(Event::Frame);
  }
}

auto VDP::step(uint clocks) -> void {
  io.hcounter += clocks;
  vdc0.dma.step(clocks); if(Model::SuperGrafx())
  vdc1.dma.step(clocks);

  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::refresh() -> void {
  screen->refresh(buffer + 1365 * 21 + 96, 1365 * sizeof(uint32), 1024, 239);
}

auto VDP::power() -> void {
  Thread::create(system.colorburst() * 6.0, {&VDP::main, this});

  vce.power();
  vdc0.power(); if(Model::SuperGrafx())
  vdc1.power(); if(Model::SuperGrafx())
  vpc.power();
}

}
#endif
