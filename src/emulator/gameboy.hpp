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
    // Caller should catch std::invalid_argument and std::runtime_exception
    Gameboy(const std::string& _rom_file_path);
    ~Gameboy();

    // Steps the components by one CPU instruction
    void step();

    std::string getRomFilePath();
    std::string getGameTitle();

    // Dumps emulated system info to the log
    void dumpSystem();

private:
    std::string rom_file_path;
    std::string game_title;

    CPU cpu;
    PPU ppu;
    Memory mem;
    Cartridge cart;
};