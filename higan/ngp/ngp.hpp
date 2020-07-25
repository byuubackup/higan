#pragma once

//license: GPLv3
//started: 2019-01-03

#include <emulator/emulator.hpp>

#include <component/processor/tlcs900h/tlcs900h.hpp>
#include <component/processor/z80/z80.hpp>
#include <component/audio/t6w28/t6w28.hpp>

namespace higan::NeoGeoPocket {
  #include <emulator/inline.hpp>

  struct Model {
    inline static auto NeoGeoPocket() -> bool;
    inline static auto NeoGeoPocketColor() -> bool;
  };

  #include <ngp/system/system.hpp>
  #include <ngp/cartridge/cartridge.hpp>
  #include <ngp/cpu/cpu.hpp>
  #include <ngp/apu/apu.hpp>
  #include <ngp/vpu/vpu.hpp>
  #include <ngp/psg/psg.hpp>
}

#include <ngp/interface/interface.hpp>
