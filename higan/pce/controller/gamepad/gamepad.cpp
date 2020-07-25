Gamepad::Gamepad(Node::Port parent, Node::Peripheral with) {
  node = Node::append<Node::Peripheral>(parent, with, "Gamepad");

  up     = Node::append<Node::Button>(node, with, "Up");
  down   = Node::append<Node::Button>(node, with, "Down");
  left   = Node::append<Node::Button>(node, with, "Left");
  right  = Node::append<Node::Button>(node, with, "Right");
  two    = Node::append<Node::Button>(node, with, "II");
  one    = Node::append<Node::Button>(node, with, "I");
  select = Node::append<Node::Button>(node, with, "Select");
  run    = Node::append<Node::Button>(node, with, "Run");
}

auto Gamepad::read() -> uint4 {
  if(clr) return 0;

  platform->input(up);
  platform->input(down);
  platform->input(left);
  platform->input(right);
  platform->input(two);
  platform->input(one);
  platform->input(select);
  platform->input(run);

  if(!(up->value() & down->value())) {
    yHold = 0, upLatch = up->value(), downLatch = down->value();
  } else if(!yHold) {
    yHold = 1, swap(upLatch, downLatch);
  }

  if(!(left->value() & right->value())) {
    xHold = 0, leftLatch = left->value(), rightLatch = right->value();
  } else if(!xHold) {
    xHold = 1, swap(leftLatch, rightLatch);
  }

  uint4 data;

  if(sel) {
    data.bit(0) = !upLatch;
    data.bit(1) = !rightLatch;
    data.bit(2) = !downLatch;
    data.bit(3) = !leftLatch;
  } else {
    data.bit(0) = !one->value();
    data.bit(1) = !two->value();
    data.bit(2) = !select->value();
    data.bit(3) = !run->value();
  }

  return data;
}

auto Gamepad::write(uint2 data) -> void {
  //there should be a small delay for this to take effect ...
  sel = data.bit(0);
  clr = data.bit(1);
}
