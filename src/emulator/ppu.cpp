#include "ppu.hpp"
#include "../program/logger.hpp"

using Logger::log, fmt::format;

PPU::PPU()
{
    LCDC = 0;
    SCX = 0;
    SCY = 0;
    STAT = 0;
    LY = 0;
    LYC = 0;
    BGP = 0;
    OBP0 = 0;
    OBP1 = 0;
    WY = 0;
    WX = 0;
    ppu_state = OAMSearch;
}

PPU::~PPU() = default;


// Steps the PPU by a given number of cycles
void PPU::step(int steps, Memory& mem)
{
    // NOTE: I'm pretty sure that certain registers are only read during certain
    // states, but this should be good enough to get something on the screen.
    readRegisters(mem);

    for(int i = 0; i < steps; i++)
    {
        switch(ppu_state)
        {
        case HBlank:
        {
            break;
        }

        case VBlank:
        {
            break;
        }

        case OAMSearch:
        {
            break;
        }

        case PixelTransfer:
        {
            break;
        }
        }
    }

    writeRegisters(mem);
}



// Dumps PPU information to the log
void PPU::dumpPPU()
{
    log("--BEGIN PPU DUMP--", Logger::logDEBUG);
    log("Register Data:", Logger::logDEBUG);
    log(format("LCDC: 0b{:08b}, STAT: 0b{:08b}", LCDC, STAT), Logger::logDEBUG);
    log(format("SCX: {:d}, SCY: {:d}", SCX, SCY), Logger::logDEBUG);
    log(format("LY: {:d}, LYC: {:d}", LY, LYC), Logger::logDEBUG);
    log(format("BGP: 0x{:02X}, OBP0: 0x{:02X}, OBP1: 0x{:02X}",
               BGP, OBP0, OBP1), Logger::logDEBUG);
    log(format("WY: {:d}, WX: {:d}", WY, WX), Logger::logDEBUG);
    log(format("PPU State: {:d}", static_cast<int>(ppu_state)),
        Logger::logDEBUG);
    log(format("--END PPU DUMP--"), Logger::logDEBUG);
}


// Reads each register's value from memory
void PPU::readRegisters(Memory& mem)
{
    LCDC = mem.readByte(0xFF40);
    SCX = mem.readByte(0xFF42);
    SCY = mem.readByte(0xFF43);
    STAT = mem.readByte(0xFF41);
    LY = mem.readByte(0xFF44);
    LYC = mem.readByte(0xFF45);
    BGP = mem.readByte(0xFF47);
    OBP0 = mem.readByte(0xFF48);
    OBP1 = mem.readByte(0xFF49);
    WY = mem.readByte(0xFF4A);
    WX = mem.readByte(0xFF4B);
}


// Writes each registers value to memory
void PPU::writeRegisters(Memory& mem) const
{
    mem.writeByte(0xFF40, LCDC);
    mem.writeByte(0xFF42, SCX);
    mem.writeByte(0xFF43, SCY);
    mem.writeByte(0xFF41, STAT);
    mem.writeByte(0xFF44, LY);
    mem.writeByte(0xFF45, LYC);
    mem.writeByte(0xFF47, BGP);
    mem.writeByte(0xFF48, OBP0);
    mem.writeByte(0xFF49, OBP1);
    mem.writeByte(0xFF4A, WY);
    mem.writeByte(0xFF4B, WX);
}