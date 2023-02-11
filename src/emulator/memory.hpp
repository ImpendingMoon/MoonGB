#pragma once

#include "../core.hpp"
#include "gbdefs.hpp"

class Memory
{
public:
    Memory();
    ~Memory();

    // Reads a byte from memory
    uint8_t readByte(uint16_t address);
    // Reads a byte from memory, ignoring PPU locks
    uint8_t readByte(uint16_t address, bool ignore_lock);
    // Reads a byte from memory, does not log. For dumping
    uint8_t getByte(uint16_t address);
    // Writes a byte to memory
    void writeByte(uint16_t address, uint8_t data);

    // Copies an array of 0x4000 bytes into ROM0
    void loadROM0(std::array<uint8_t, 0x4000> data);
    // Copies a vector of MemoryBanks into ROM1
    void loadROM1(std::vector<MemoryBank> data);
    // Sets the currently selected ROM1 bank
    void setROM1Index(uint8_t index);
    // Sets the currently selected ERAM bank
    void setERAMIndex(uint8_t index);

    // Dumps the contents of memory to the log
    void dumpMemory();

private:
    MemoryBank ROM0{{}, false};
    std::vector<MemoryBank> ROM1{}; // MBC controls bank amount
    uint8_t ROM1_index = 0;
    std::array<MemoryBank, 2> VRAM{}; // CGB has 2 banks of VRAM
    uint8_t VRAM_index = 0;
    // ERAM handled by cartridge
    uint8_t ERAM_index = 0;
    MemoryBank WRAM0{{}, false};
    std::array<MemoryBank, 7> WRAM1{}; // CGB has 7 banks of WRAM
    uint8_t WRAM1_index = 0;
    MemoryBank OAM{{}, false};
    MemoryBank IOReg{{}, false};
    MemoryBank HRAM{{}, false};
    MemoryBank IEReg{{}, false};
};