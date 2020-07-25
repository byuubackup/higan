auto SA1::ROM::conflict() const -> bool {
  if((cpu.r.mar & 0x408000) == 0x008000) return true;  //00-3f,80-bf:8000-ffff
  if((cpu.r.mar & 0xc00000) == 0xc00000) return true;  //c0-ff:0000-ffff
  return false;
}

auto SA1::ROM::read(uint24 address, uint8 data) -> uint8 {
  address = bus.mirror(address, size());
  return ReadableMemory::read(address, data);
}

auto SA1::ROM::write(uint24 address, uint8 data) -> void {
}

//note: addresses are translated prior to invoking this function:
//00-3f,80-bf:8000-ffff mask=0x408000 => 00-3f:0000-ffff
//c0-ff:0000-ffff => untranslated
auto SA1::ROM::readCPU(uint24 address, uint8 data) -> uint8 {
  //reset vector overrides
  if((address & 0xffffe0) == 0x007fe0) {  //00:ffe0-ffef
    if(address == 0x7fea && sa1.io.cpu_nvsw) return sa1.io.snv >> 0;
    if(address == 0x7feb && sa1.io.cpu_nvsw) return sa1.io.snv >> 8;
    if(address == 0x7fee && sa1.io.cpu_ivsw) return sa1.io.siv >> 0;
    if(address == 0x7fef && sa1.io.cpu_ivsw) return sa1.io.siv >> 8;
  }

  static auto read = [](uint address) {
    if((address & 0x400000) && bsmemory.size()) return bsmemory.read(address, 0x00);
    return sa1.rom.read(address);
  };

  bool lo = address < 0x400000;  //*bmode==0 only applies to 00-3f,80-bf:8000-ffff
  address &= 0x3fffff;

  if(address < 0x100000) {  //00-1f,8000-ffff; c0-cf:0000-ffff
    if(lo && sa1.io.cbmode == 0) return read(address);
    return read(sa1.io.cb << 20 | address & 0x0fffff);
  }

  if(address < 0x200000) {  //20-3f,8000-ffff; d0-df:0000-ffff
    if(lo && sa1.io.dbmode == 0) return read(address);
    return read(sa1.io.db << 20 | address & 0x0fffff);
  }

  if(address < 0x300000) {  //80-9f,8000-ffff; e0-ef:0000-ffff
    if(lo && sa1.io.ebmode == 0) return read(address);
    return read(sa1.io.eb << 20 | address & 0x0fffff);
  }

  if(address < 0x400000) {  //a0-bf,8000-ffff; f0-ff:0000-ffff
    if(lo && sa1.io.fbmode == 0) return read(address);
    return read(sa1.io.fb << 20 | address & 0x0fffff);
  }

  return data;  //unreachable
}

auto SA1::ROM::writeCPU(uint24 address, uint8 data) -> void {
}

auto SA1::ROM::readSA1(uint24 address, uint8 data) -> uint8 {
  if((address & 0x408000) == 0x008000) {
    address = (address & 0x800000) >> 2 | (address & 0x3f0000) >> 1 | address & 0x007fff;
  }
  return readCPU(address, data);
}

auto SA1::ROM::writeSA1(uint24 address, uint8 data) -> void {
}
