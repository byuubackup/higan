//immediate, 2-cycle opcodes with idle cycle will become bus read
//when an IRQ is to be triggered immediately after opcode completion.
//this affects the following opcodes:
//  clc, cld, cli, clv, sec, sed, sei,
//  tax, tay, txa, txy, tya, tyx,
//  tcd, tcs, tdc, tsc, tsx, txs,
//  inc, inx, iny, dec, dex, dey,
//  asl, lsr, rol, ror, nop, xce.
auto WDC65816::idleIRQ() -> void {
  if(interruptPending()) {
    //modify I/O cycle to bus read cycle, do not increment PC
    read(PC.d);
  } else {
    idle();
  }
}

auto WDC65816::idle2() -> void {
  if(D.l) idle();
}

auto WDC65816::idle4(uint16 x, uint16 y) -> void {
  if(!XF || x >> 8 != y >> 8) idle();
}

auto WDC65816::idle6(uint16 address) -> void {
  if(EF && PC.h != address >> 8) idle();
}

auto WDC65816::fetch() -> uint8 {
  return read(PC.b << 16 | PC.w++);
}

auto WDC65816::pull() -> uint8 {
  EF ? (void)S.l++ : (void)S.w++;
  return read(S.w);
}

auto WDC65816::push(uint8 data) -> void {
  write(S.w, data);
  EF ? (void)S.l-- : (void)S.w--;
}

auto WDC65816::pullN() -> uint8 {
  return read(++S.w);
}

auto WDC65816::pushN(uint8 data) -> void {
  write(S.w--, data);
}

auto WDC65816::readDirect(uint address) -> uint8 {
  if(EF && !D.l) return read(D.w | uint8(address));
  return read(uint16(D.w + address));
}

auto WDC65816::writeDirect(uint address, uint8 data) -> void {
  if(EF && !D.l) return write(D.w | uint8(address), data);
  write(uint16(D.w + address), data);
}

auto WDC65816::readDirectN(uint address) -> uint8 {
  return read(uint16(D.w + address));
}

auto WDC65816::readBank(uint address) -> uint8 {
  return read((B << 16) + address);
}

auto WDC65816::writeBank(uint address, uint8 data) -> void {
  write((B << 16) + address, data);
}

auto WDC65816::readStack(uint address) -> uint8 {
  return read(uint16(S.w + address));
}

auto WDC65816::writeStack(uint address, uint8 data) -> void {
  write(uint16(S.w + address), data);
}
