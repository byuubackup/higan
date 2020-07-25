struct Controller : Thread {
  virtual ~Controller() = default;

  virtual auto read() -> uint8 { return 0xff; }
  virtual auto write(uint8 data) -> void {}

protected:
  Node::Peripheral node;
};

#include "port.hpp"
#include "gamepad/gamepad.hpp"
