#include <gb/gb.hpp>

namespace higan::GameBoy {

Scheduler scheduler;
System system;
#include "controls.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Event::Frame) ppu.refresh();
}

auto System::clocksExecuted() -> uint {
  uint clocks = information.clocksExecuted;
  information.clocksExecuted = 0;
  return clocks;
}

auto System::load(Node::Object& root, Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "Game Boy") {
    information.model = Model::GameBoy;
    node = Node::append<Node::System>(nullptr, from, interface->name());
    root = node;
  }
  if(interface->name() == "Super Game Boy") {
    information.model = Model::SuperGameBoy;
    node = root;
  }
  if(interface->name() == "Game Boy Color") {
    information.model = Model::GameBoyColor;
    node = Node::append<Node::System>(nullptr, from, interface->name());
    root = node;
  }

  scheduler.reset();
  controls.load(node, from);
  cpu.load(node, from);
  ppu.load(node, from);
  apu.load(node, from);
  cartridge.load(node, from);
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cpu.unload();
  ppu.unload();
  apu.unload();
  cartridge.unload();
  bootROM.reset();
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  string name = "boot.rom";  //fallback name (should not be used)

  if(GameBoy::Model::GameBoy()) {
    bootROM.allocate(256);
    if(cpu.version->latch() == "DMG-CPU"   ) name = "boot.dmg-0.rom";
    if(cpu.version->latch() == "DMG-CPU A" ) name = "boot.dmg-1.rom";
    if(cpu.version->latch() == "DMG-CPU B" ) name = "boot.dmg-1.rom";
    if(cpu.version->latch() == "DMG-CPU C" ) name = "boot.dmg-1.rom";
    if(cpu.version->latch() == "CPU MGB"   ) name = "boot.mgb.rom";
  }

  if(GameBoy::Model::SuperGameBoy()) {
    bootROM.allocate(256);
    if(cpu.version->latch() == "SGB-CPU 01") name = "sm83.boot.rom";
    if(cpu.version->latch() == "CPU SGB2"  ) name = "sm83.boot.rom";
  }

  if(GameBoy::Model::GameBoyColor()) {
    bootROM.allocate(2048);
    if(cpu.version->latch() == "CPU CGB"   ) name = "boot.cgb-0.rom";
    if(cpu.version->latch() == "CPU CGB A" ) name = "boot.cgb-1.rom";
    if(cpu.version->latch() == "CPU CGB B" ) name = "boot.cgb-1.rom";
    if(cpu.version->latch() == "CPU CGB C" ) name = "boot.cgb-1.rom";
    if(cpu.version->latch() == "CPU CGB D" ) name = "boot.cgb-1.rom";
    if(cpu.version->latch() == "CPU CGB E" ) name = "boot.cgb-1.rom";
  }

  if(auto fp = platform->open(node, name, File::Read, File::Required)) {
    bootROM.load(fp);
  }

  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.power(cpu);

  information.serializeSize[0] = serializeInit(0);
  information.serializeSize[1] = serializeInit(1);
}

}
