struct Input;

struct InputDriver {
  InputDriver(Input& super) : super(super) {}
  virtual ~InputDriver() = default;

  virtual auto create() -> bool { return true; }
  virtual auto driver() -> string { return "None"; }
  virtual auto ready() -> bool { return true; }

  virtual auto hasContext() -> bool { return false; }

  virtual auto setContext(uintptr context) -> bool { return true; }

  virtual auto acquired() -> bool { return false; }
  virtual auto acquire() -> bool { return false; }
  virtual auto release() -> bool { return false; }
  virtual auto poll() -> vector<shared_pointer<nall::HID::Device>> { return {}; }
  virtual auto rumble(uint64_t id, bool enable) -> bool { return false; }

protected:
  Input& super;
  friend class Input;

  uintptr context = 0;
};

struct Input {
  static auto hasDrivers() -> vector<string>;
  static auto hasDriver(string driver) -> bool { return (bool)hasDrivers().find(driver); }
  static auto optimalDriver() -> string;
  static auto safestDriver() -> string;

  Input() : self(*this) { reset(); }
  explicit operator bool() { return instance->driver() != "None"; }
  auto reset() -> void { instance = new InputDriver(*this); }
  auto create(string driver = "") -> bool;
  auto driver() -> string { return instance->driver(); }
  auto ready() -> bool { return instance->ready(); }

  auto hasContext() -> bool { return instance->hasContext(); }

  auto context() -> uintptr { return instance->context; }

  auto setContext(uintptr context) -> bool;

  auto acquired() -> bool;
  auto acquire() -> bool;
  auto release() -> bool;
  auto poll() -> vector<shared_pointer<nall::HID::Device>>;
  auto rumble(uint64_t id, bool enable) -> bool;

  auto onChange(const function<void (shared_pointer<nall::HID::Device>, uint, uint, int16_t, int16_t)>&) -> void;
  auto doChange(shared_pointer<nall::HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;

protected:
  Input& self;
  unique_pointer<InputDriver> instance;
  function<void (shared_pointer<nall::HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue)> change;
};
