//Programmable Sound Generator

struct PSG : Thread {
  Node::Component node;
  Node::Stream stream;

  //psg.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;

  auto main() -> void;
  auto frame(int16&, int16&) -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //io.cpp
  auto write(uint4 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct IO {
    uint3 channel;
    uint4 volumeLeft;
    uint4 volumeRight;
    uint8 lfoFrequency;
    uint2 lfoControl;
    uint1 lfoEnable;
  } io;

  struct Channel {
    //channel.cpp
    auto power(uint id) -> void;
    auto run() -> void;
    auto sample(uint5 sample) -> void;

    //io.cpp
    auto write(uint4 address, uint8 data) -> void;

    struct IO {
      uint12 waveFrequency;
       uint5 volume;
       uint1 direct;
       uint1 enable;
       uint4 volumeLeft;
       uint4 volumeRight;
       uint5 waveBuffer[32];
       uint5 noiseFrequency;  //channels 4 and 5 only
       uint1 noiseEnable;     //channels 4 and 5 only

      uint12 wavePeriod;
       uint5 waveSample;
       uint5 waveOffset;
      uint12 noisePeriod;
       uint5 noiseSample;

       uint5 output;
    } io;

    uint id;
  } channel[6];

//unserialized:
  double volumeScalar[32];
};

extern PSG psg;
