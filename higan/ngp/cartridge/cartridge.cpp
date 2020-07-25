#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

Cartridge cartridge;
#include "flash.cpp"
#include "serialization.cpp"

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::append<Node::Port>(parent, from, "Cartridge Slot");
  port->setFamily(interface->name());
  port->setType("Cartridge");
  port->setAllocate([&] { return Node::Peripheral::create(interface->name()); });
  port->setAttach([&](auto node) { connect(node); });
  port->setDetach([&](auto node) { disconnect(); });
  port->scan(from);
}

auto Cartridge::unload() -> void {
  disconnect();
  port = {};
}

auto Cartridge::connect(Node::Peripheral with) -> void {
  node = Node::append<Node::Peripheral>(port, with, interface->name());
  node->setManifest([&] { return information.manifest; });

  information = {};

  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  }

  auto document = BML::unserialize(information.manifest);
  information.name = document["game/label"].text();

  flash[0].reset(0);
  flash[1].reset(1);

  if(auto memory = document["game/board/memory(type=Flash,content=Program)"]) {
    auto size = memory["size"].natural();
    flash[0].allocate(min(16_Mibit, size));
    flash[1].allocate(size >= 16_Mibit ? size - 16_Mibit : 0);
    if(auto fp = platform->open(node, "program.flash", File::Read, File::Required)) {
      flash[0].load(fp);
      flash[1].load(fp);
    };
  }

  power();
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  flash[0].reset(0);
  flash[1].reset(1);
}

auto Cartridge::save() -> void {
  if(!node) return;
  auto document = BML::unserialize(information.manifest);

  if(auto memory = document["game/board/memory(type=Flash,content=Program)"]) {
    if(flash[0].modified || flash[1].modified) {
      if(auto fp = platform->open(node, "program.flash", File::Write)) {
        flash[0].save(fp);
        flash[1].save(fp);
      }
    }
  }
}

auto Cartridge::power() -> void {
  flash[0].power();
  flash[1].power();
}

auto Cartridge::read(uint1 chip, uint21 address) -> uint8 {
  if(!flash[chip]) return 0x00;
  return flash[chip].read(address);
}

auto Cartridge::write(uint1 chip, uint21 address, uint8 data) -> void {
  if(!flash[chip]) return;
  return flash[chip].write(address, data);
}

}
