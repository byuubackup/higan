#if defined(CORE_PCE)

namespace higan::PCEngine {

extern Interface* interface;

struct AbstractInterface : Interface {
  auto game() -> string override;

  auto root() -> Node::Object override;
  auto load(Node::Object&, string tree = {}) -> void override;
  auto unload() -> void override;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize(bool synchronize) -> serializer override;
  auto unserialize(serializer&) -> bool override;
};

struct PCEngineInterface : AbstractInterface {
  auto name() -> string override { return "PC Engine"; }
};

struct SuperGrafxInterface : AbstractInterface {
  auto name() -> string override { return "SuperGrafx"; }
};

}

#endif
