// The main emulator object, contains and connects all the components
#pragma once

#include "../core.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"

class Gameboy
{
public:
    Gameboy();
    ~Gameboy();

    // Steps the components by one CPU instruction
    void step();

    // Dumps emulated system info to the log
    void dumpSystem();

private:

};