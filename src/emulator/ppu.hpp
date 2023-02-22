#pragma once

#include "../core.hpp"
#include <queue>
#include "memory.hpp"
#include "../program/window.hpp"

class PPU
{
public:
    PPU();
    ~PPU();

    // Steps the PPU by a given number of cycles
    void step(int steps, Memory& mem);

    // Dumps PPU information to the log
    void dumpPPU();

private:

    enum PPUState
    {
        HBlank = 0,
        VBlank = 1,
        OAMSearch = 2,
        PixelTransfer = 3,
    } ppu_state;

    uint16_t scanl_cycle; // Current cycle in the scanline

    uint8_t LCDC; // LCD Control - $FF40
    uint8_t SCX, SCY; // Scroll X and Y - $FF42 and $FF43
    uint8_t STAT; // LCD Status - $FF41
    uint8_t LY, LYC; // Line Y and Line Y Compare - $FF44 and $FF45
    uint8_t BGP; // Background Palette Data - $FF47
    uint8_t OBP0, OBP1; // Object Palette Data - $FF48 and $FF49
    uint8_t WY, WX; // Window Y and X - $FF4A and $FF4B

    std::queue<uint8_t> pixel_fifo{};
    Window::PImage frame_buffer = {{}, 160, 144};

    // Reads each registers' value from memory
    void readRegisters(Memory& mem);
    // Writes each registers' value to memory
    void writeRegisters(Memory& mem) const;
};