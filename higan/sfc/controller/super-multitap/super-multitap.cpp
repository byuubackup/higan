SuperMultitap::SuperMultitap(Node::Port parent, Node::Peripheral with):
port1{"Controller Port 1"},
port2{"Controller Port 2"},
port3{"Controller Port 3"},
port4{"Controller Port 4"}
{
  node = Node::append<Node::Peripheral>(parent, with, "Super Multitap");

  port1.load(node, with);
  port2.load(node, with);
  port3.load(node, with);
  port4.load(node, with);
}

auto SuperMultitap::data() -> uint2 {
  if(latched) return 2;  //Super Multitap device detection

  if(iobit()) {
    return port1.data().bit(0) << 0 | port2.data().bit(0) << 1;
  } else {
    return port3.data().bit(0) << 0 | port4.data().bit(0) << 1;
  }
}

auto SuperMultitap::latch(bool data) -> void {
  latched = data;
  port1.latch(data);
  port2.latch(data);
  port3.latch(data);
  port4.latch(data);
}
