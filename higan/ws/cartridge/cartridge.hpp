struct Cartridge : Thread, IO {
  Node::Port port;
  Node::Peripheral node;

  inline auto manifest() const { return information.manifest; }
  inline auto name() const { return information.name; }
  inline auto orientation() const { return information.orientation; }

  //cartridge.cpp
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;
  auto save() -> void;
  auto power() -> void;

  //memory.cpp
  auto romRead(uint20 address) -> uint8;
  auto romWrite(uint20 address, uint8 data) -> void;

  auto ramRead(uint20 address) -> uint8;
  auto ramWrite(uint20 address, uint8 data) -> void;

  //rtc.cpp
  auto rtcLoad() -> void;
  auto rtcSave() -> void;
  auto rtcTickSecond() -> void;
  auto rtcCheckAlarm() -> void;
  auto rtcStatus() -> uint8;
  auto rtcCommand(uint8 data) -> void;
  auto rtcRead() -> uint8;
  auto rtcWrite(uint8 data) -> void;

  //io.cpp
  auto portRead(uint16 address) -> uint8 override;
  auto portWrite(uint16 address, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    string manifest;
    string name;
    string orientation = "Horizontal";
  } information;

  struct Registers {
    //$00c0  BANK_ROM2
    uint8 romBank2 = 0xff;

    //$00c1  BANK_SRAM
    uint8 sramBank = 0xff;

    //$00c2  BANK_ROM0
    uint8 romBank0 = 0xff;

    //$00c3  BANK_ROM1
    uint8 romBank1 = 0xff;

    //$00cc  GPO_EN
    uint8 gpoEnable;

    //$00cd  GPO_DATA
    uint8 gpoData;
  } r;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
    uint mask = 0;
  };

  struct RTC : Memory {
    uint8 command;
    uint4 index;

    uint8 alarm;
    uint8 alarmHour;
    uint8 alarmMinute;

    auto year()    -> uint8& { return data[0]; }
    auto month()   -> uint8& { return data[1]; }
    auto day()     -> uint8& { return data[2]; }
    auto weekday() -> uint8& { return data[3]; }
    auto hour()    -> uint8& { return data[4]; }
    auto minute()  -> uint8& { return data[5]; }
    auto second()  -> uint8& { return data[6]; }
  };

  Memory rom;
  Memory ram;
  EEPROM eeprom;
  RTC rtc;
};

extern Cartridge cartridge;
