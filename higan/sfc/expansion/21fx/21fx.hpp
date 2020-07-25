struct S21FX : Expansion {
  Node::Peripheral node;

  S21FX(Node::Port, Node::Peripheral);
  ~S21FX();

  auto step(uint clocks) -> void;
  auto main() -> void;

  auto read(uint24 address, uint8 data) -> uint8;
  auto write(uint24 address, uint8 data) -> void;

private:
  auto quit() -> bool;
  auto usleep(uint) -> void;
  auto readable() -> bool;
  auto writable() -> bool;
  auto read() -> uint8;
  auto write(uint8) -> void;

  bool booted = false;
  uint16 resetVector;
  uint8 ram[122];

  nall::library link;
  function<void (
    function<bool ()>,      //quit
    function<void (uint)>,  //usleep
    function<bool ()>,      //readable
    function<bool ()>,      //writable
    function<uint8 ()>,     //read
    function<void (uint8)>  //write
  )> linkInit;
  function<void (vector<string>)> linkMain;

  vector<uint8> snesBuffer;  //SNES -> Link
  vector<uint8> linkBuffer;  //Link -> SNES
};
