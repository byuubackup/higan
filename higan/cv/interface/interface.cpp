#include <cv/cv.hpp>

namespace higan::ColecoVision {

Interface* interface = nullptr;

auto AbstractInterface::game() -> string {
  if(cartridge.node) {
    return cartridge.name();
  }

  return "(no cartridge connected)";
}

auto AbstractInterface::root() -> Node::Object {
  return system.node;
}

auto AbstractInterface::load(Node::Object& root, string tree) -> void {
  interface = this;
  system.load(root, Node::unserialize(tree));
}

auto AbstractInterface::power() -> void {
  system.power();
}

auto AbstractInterface::run() -> void {
  system.run();
}

auto AbstractInterface::save() -> void {
  system.save();
}

auto AbstractInterface::unload() -> void {
  save();
  system.unload();
}

auto AbstractInterface::serialize(bool synchronize) -> serializer {
  return system.serialize(synchronize);
}

auto AbstractInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

}
