#include <msx/msx.hpp>

namespace higan::MSX {

Keyboard keyboard;
#include "serialization.cpp"

auto Keyboard::load(Node::Object parent, Node::Object from) -> void {
  port = Node::append<Node::Port>(parent, from, "Keyboard");
  port->setFamily("MSX");
  port->setType("Keyboard");
  port->setHotSwappable(true);
  port->setAttach([&](auto node) { connect(node); });
  port->setDetach([&](auto node) { disconnect(); });
  port->scan(from);
}

auto Keyboard::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    string name{"Layout"};
    if(node) name = node->name();
    layout = Node::append<Node::Peripheral>(port, node, name);
    Markup::Node document;
    if(auto fp = platform->open(layout, "layout.bml", File::Read)) {
      document = BML::unserialize(fp->reads());
    }
    for(uint column : range(12)) {
      for(uint row : range(8)) {
        string label{column, ",", row};
        if(auto key = document[{"layout/key[", column * 8 + row, "]"}]) {
          label = key.text();
        }
        matrix[column][row] = Node::append<Node::Button>(layout, node, label);
      }
    }
  }
}

auto Keyboard::disconnect() -> void {
  layout = {};
  for(uint column : range(12)) {
    for(uint row : range(8)) {
      matrix[column][row] = {};
    }
  }
}

auto Keyboard::power() -> void {
  io = {};
}

auto Keyboard::read() -> uint8 {
  uint8 column = io.select;
  uint8 data = 0xff;
  if(column >= 0 && column <= 11) {
    for(uint row : range(8)) {
      if(auto node = matrix[column][row]) {
        platform->input(node);
        data.bit(row) = !node->value();
      }
    }
  }
  return data;
}

auto Keyboard::write(uint4 data) -> void {
  io.select = data;
}

}
