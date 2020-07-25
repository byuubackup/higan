struct MSU1 : Thread {
  Node::Stream stream;
  Shared::File dataFile;
  Shared::File audioFile;

  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;

  auto main() -> void;
  auto power() -> void;

  auto dataOpen() -> void;
  auto audioOpen() -> void;

  auto readIO(uint24 address, uint8 data) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;

  auto serialize(serializer&) -> void;

private:
  enum Flag : uint {
    Revision       = 0x02,  //max: 0x07
    AudioError     = 0x08,
    AudioPlaying   = 0x10,
    AudioRepeating = 0x20,
    AudioBusy      = 0x40,
    DataBusy       = 0x80,
  };

  struct IO {
    uint32 dataSeekOffset;
    uint32 dataReadOffset;

    uint32 audioPlayOffset;
    uint32 audioLoopOffset;

    uint16 audioTrack;
    uint8 audioVolume;

    uint32 audioResumeTrack;
    uint32 audioResumeOffset;

    boolean audioError;
    boolean audioPlay;
    boolean audioRepeat;
    boolean audioBusy;
    boolean dataBusy;
  } io;
};

extern MSU1 msu1;
