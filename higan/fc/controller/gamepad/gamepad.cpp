Gamepad::Gamepad(Node::Port parent, Node::Peripheral with) {
  node = Node::append<Node::Peripheral>(parent, with, "Gamepad");

  up     = Node::append<Node::Button>(node, with, "Up");
  down   = Node::append<Node::Button>(node, with, "Down");
  left   = Node::append<Node::Button>(node, with, "Left");
  right  = Node::append<Node::Button>(node, with, "Right");
  b      = Node::append<Node::Button>(node, with, "B");
  a      = Node::append<Node::Button>(node, with, "A");
  select = Node::append<Node::Button>(node, with, "Select");
  start  = Node::append<Node::Button>(node, with, "Start");
}

auto Gamepad::data() -> uint3 {
  if(latched == 1) {
    platform->input(a);
    return a->value();
  }

  switch(counter++) {
  case 0: return a->value();
  case 1: return b->value();
  case 2: return select->value();
  case 3: return start->value();
  case 4: return upLatch;
  case 5: return downLatch;
  case 6: return leftLatch;
  case 7: return rightLatch;
  }

  counter = 8;
  return 1;
}

auto Gamepad::latch(uint1 data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    platform->input(a);
    platform->input(b);
    platform->input(select);
    platform->input(start);
    platform->input(up);
    platform->input(down);
    platform->input(left);
    platform->input(right);

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
  }
}
