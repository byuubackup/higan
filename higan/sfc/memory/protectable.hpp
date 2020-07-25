struct ProtectableMemory : AbstractMemory {
  inline auto reset() -> void override {
    delete[] self.data;
    self.data = nullptr;
    self.size = 0;
  }

  inline auto allocate(uint size, uint8 fill = 0xff) -> void override {
    delete[] self.data;
    self.data = new uint8[self.size = size];
    for(uint address : range(size)) self.data[address] = fill;
  }

  inline auto load(shared_pointer<vfs::file> fp) -> void {
    fp->read(self.data, min(fp->size(), self.size));
  }

  inline auto save(shared_pointer<vfs::file> fp) -> void {
    fp->write(self.data, self.size);
  }

  inline auto data() -> uint8* override {
    return self.data;
  }

  inline auto size() const -> uint override {
    return self.size;
  }

  inline auto writable() const -> bool {
    return self.writable;
  }

  inline auto writable(bool writable) -> void {
    self.writable = writable;
  }

  inline auto read(uint24 address, uint8 data = 0) -> uint8 override {
    return self.data[address];
  }

  inline auto write(uint24 address, uint8 data) -> void override {
    if(!self.writable) return;
    self.data[address] = data;
  }

  inline auto operator[](uint24 address) const -> uint8 {
    return self.data[address];
  }

private:
  struct {
    uint8* data = nullptr;
    uint size = 0;
    bool writable = false;
  } self;
};
