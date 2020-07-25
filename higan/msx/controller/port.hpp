struct ControllerPort {
  //port.cpp
  ControllerPort(string_view name);
  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto read() -> uint6 { if(device) return device->read(); return 0x3f; }
  auto write(uint8 data) { if(device) return device->write(data); }

  auto serialize(serializer&) -> void;

  const string name;
  Node::Port port;
  unique_pointer<Controller> device;
};

extern ControllerPort controllerPort1;
extern ControllerPort controllerPort2;
