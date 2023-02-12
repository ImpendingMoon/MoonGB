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

    std::string getRomFilePath() const;
    std::string getGameTitle() const;

    int getCycle() const; // Gets the current cycle in the frame
    int getCyclesPerFrame() const; // Gets the number of cycles in a frame
    void resetCycle(); // Wraps the cycles back to 0

    // Dumps emulated system info to the log
    void dumpSystem();

private:
    std::string rom_file_path;
    std::string game_title;

    int cycles_per_frame;
    int cycle;

    CPU cpu;
    PPU ppu;
    Memory mem;
    Cartridge cart;
};