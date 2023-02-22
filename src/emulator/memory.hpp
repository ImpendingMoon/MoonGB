#pragma once

#include "../core.hpp"
#include "gbdefs.hpp"
#include <fstream>

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
    void loadROM0(const std::array<uint8_t, 0x4000>& data);
    // Copies a vector of MemoryBanks into ROM1
    void loadROM1(const std::vector<MemoryBank>& data, uint16_t bank_amount);
    // Sets the currently selected ROM1 bank
    void setROM1Index(const uint8_t& index);
    // Sets up the ERAM
    void setERAM(const uint16_t& _bank_amount,
                      bool _persistent,
                      const std::string& _sav_file_path,
                      BankController mbc);

    // Sets locks for PPU
    void setVRAMLock(bool value);
    void setOAMLock(bool value);

    // Dumps the contents of memory to the log
    void dumpMemory();

private:
    MemoryBank ROM0{{}, false};
    std::vector<MemoryBank> ROM1{}; // MBC controls bank amount
    uint16_t ROM1_index = 0;
    uint16_t ROM_bank_amount;
    std::array<MemoryBank, 2> VRAM{}; // CGB has 2 banks of VRAM
    uint8_t VRAM_index = 0;
    // ERAM handled further in file
    uint16_t ERAM_index = 0;
    MemoryBank WRAM0{{}, false};
    std::array<MemoryBank, 7> WRAM1{}; // CGB has 7 banks of WRAM
    uint8_t WRAM1_index = 0;
    MemoryBank OAM{{}, false};
    MemoryBank IOReg{{}, false};
    MemoryBank HRAM{{}, false};
    MemoryBank IEReg{{}, false};

    BankController mbc;
    uint16_t ERAM_bank_amount;
    bool ERAM_persistent;
    // Vector is used if ERAM is not persistent
    std::vector<uint8_t> np_ERAM;
    // fstream is used if ERAM is persistent
    std::string sav_file_path;
    std::fstream p_ERAM;

    inline uint8_t readERAMByte(uint8_t bank, uint16_t address);
    inline void writeERAMByte(uint8_t bank, uint16_t address, uint8_t data);
};