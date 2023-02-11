#pragma once

#include "../core.hpp"
#include "gbdefs.hpp"
#include "memory.hpp"
#include <fstream>

class Memory;

class Cartridge
{
public:
    Cartridge();
    ~Cartridge();

    // Initializes the file(s), performs checks, and gets the game title,
    // memory bank controller, and size of the ROM from the header.
    void initCartridge(const std::string& _rom_file_path);

    // Loads the initialized cartridge into the specified memory object.
    void loadCartridge(Memory& mem);


    std::string getROMFilePath();
    std::string getSAVFilePath();
    std::string getGameTitle();

    // Writes cartridge information to the log
    void dumpCartridge();

private:
    std::string rom_file_path{};
    std::string sav_file_path{};
    std::ifstream RomFile;

    std::string game_title{};

    BankController mbc;
    uint16_t rom_bank_amount;
    uint16_t ram_bank_amount;
    bool persistent_memory;

    // Returns if the header checksum is valid
    static bool checkHeader(std::array<uint8_t, 80> header);
    // Pulls the ASCII game title from the ROM header
    static std::string parseGameTitle(std::array<uint8_t, 80> header);
};
