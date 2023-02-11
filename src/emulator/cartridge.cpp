#include "cartridge.hpp"
#include <filesystem>
#include <exception>

using Logger::log, std::string, fmt::format;

Cartridge::Cartridge() = default;

Cartridge::~Cartridge()
{
    if(RomFile.is_open()) { RomFile.close(); }
}

// Initializes the file(s), performs checks, and gets the game title,
// memory bank controller, and size of the ROM from the header.
void Cartridge::initCartridge(const string& _rom_file_path)
{
    using namespace std::filesystem;

    if(!exists(_rom_file_path))
    {
        throw std::invalid_argument("File does not exist!");
    }

    // Check if file is .gb or .gbc
    size_t extension_pos = _rom_file_path.find_last_of('.');
    string rom_extension = _rom_file_path.substr(extension_pos);
    if(!(rom_extension == ".gb" || rom_extension == ".gbc"))
    {
        throw std::invalid_argument("File is not a .gb or .gbc ROM!");
    }

    RomFile.open(_rom_file_path, std::ios_base::in | std::ios_base::binary);

    if(!RomFile)
    {
        throw std::runtime_error("Could not open file!");
    }

    rom_file_path = _rom_file_path;

    // Pull info from header

    // Read the ROM's header into a byte array
    std::array<uint8_t, 80> header{}; // Header is 80 bytes between $100-$14F
    try {
        RomFile.seekg(0x100);
        RomFile.read((char*)(header.data()), 80);
        RomFile.seekg(0); // Reset position

    } catch(std::exception& ex) {
        Logger::log("CART: Could not read ROM Header.",
                    Logger::logDEBUG);
        Logger::log(ex.what(), Logger::logDEBUG);

        // Rethrow with more descriptive message
        throw std::runtime_error("ROM is corrupt: Could not read ROM Header.");
    }

    // Check the validity of the ROM's header
    if(!checkHeader(header))
    {
        throw std::runtime_error("ROM is corrupt: Bad header checksum.");
    }

    game_title = parseGameTitle(header);

    // MBC Identifier at address $0147
    mbc = static_cast<BankController>(header.at(0x47));

    // Check to see if external RAM is persistent
    switch(mbc)
    {
        case NONE_BAT_RAM: case MBC1_BAT_RAM: case MBC2_BAT: case MBC3_BAT_RAM:
        case MBC3_BAT_RAM_TIMER: case MBC5_BAT_RAM: case MBC5_RUMBLE_BAT_RAM:
        {
            persistent_memory = true;
            // Create file path with extension .sav
            sav_file_path = rom_file_path;
            sav_file_path.replace(extension_pos, sav_file_path.length(), ".sav");

            break;
        }
        default:
        {
            persistent_memory = false;
        }
    }

    // Get the ROM and RAM bank amounts
    switch(header.at(0x48))
    {
        case 0x00: rom_bank_amount = 2; break;
        case 0x01: rom_bank_amount = 4; break;
        case 0x02: rom_bank_amount = 8; break;
        case 0x03: rom_bank_amount = 16; break;
        case 0x04: rom_bank_amount = 32; break;
        case 0x05: rom_bank_amount = 64; break;
        case 0x06: rom_bank_amount = 128; break;
        case 0x07: rom_bank_amount = 256; break;
        case 0x08: rom_bank_amount = 512; break;
        default:
        {
            throw std::runtime_error("ROM is corrupt: Invalid ROM size in header.");
        }
            // 0x52, 0x53, and 0x54 might exist, but probably don't.
    }

    switch(header.at(0x49))
    {
        case 0x00: ram_bank_amount = 0; break;
        // 0x01 is unused
        case 0x02: ram_bank_amount = 1; break;
        case 0x03: ram_bank_amount = 4; break;
        case 0x04: ram_bank_amount = 16; break;
        case 0x05: ram_bank_amount = 8; break;
        default:
        {
            throw std::runtime_error("ROM is corrupt: Invalid RAM size in header.");
        }
    }

    // Now ready to call loadCartridge to load the game
}



// Loads the initialized cartridge into the specified memory object.
void Cartridge::loadCartridge(Memory& mem)
{
    // Send save info to memory
    mem.setERAM(rom_bank_amount,
                persistent_memory,
                sav_file_path,
                mbc
    );


    // Begin loading ROM into memory
    try {
        std::array<uint8_t, 0x4000> static_rom{};
        RomFile.seekg(0);
        RomFile.read((char*)(static_rom.data()), static_rom.size());

        mem.loadROM0(static_rom);

    } catch(std::exception& ex) {

        throw std::runtime_error("ROM is corrupt: Could not read static ROM");
    }

    // Load ROM banks into memory
    std::vector<MemoryBank> rom_banks{};
    rom_banks.reserve(rom_bank_amount);
    // Iterate through every bank, reading 16kb chunks into memory
    // Subtract 1 because Static ROM (bank 1) is already loaded
    for(int i = 0; i < rom_bank_amount - 1; i++)
    {
        try {
            MemoryBank bank{};
            bank.data.reserve(0x4000);
            RomFile.read((char*)(bank.data.data()), 0x4000);
            rom_banks.push_back(bank);

        } catch(std::exception& ex) {
            throw std::runtime_error(
                    "ROM is corrupt: Could not read all ROM banks.");
        }
    }

    // Send banks to MMU
    mem.loadROM1(rom_banks, rom_bank_amount);

    // RomFile is no longer needed.
    RomFile.close();
}



string Cartridge::getROMFilePath() { return rom_file_path; }
string Cartridge::getSAVFilePath() { return sav_file_path; }
string Cartridge::getGameTitle() { return game_title; }



// Returns if the header checksum is valid
bool Cartridge::checkHeader(std::array<uint8_t, 80> header)
{
    // The checked header resides in 25 bytes from $134-$14C
    // The checksum value is at $14D
    std::array<uint8_t, 25> checked_header{};
    std::copy(
            header.begin() + 0x34,
            header.begin() + 0x4C + 1,
            checked_header.begin()
    );
    uint8_t checksum = header.at(0x4D);

    // The checksum is calculated by adding the inverse of all values inside
    // the checked header.
    uint8_t sum = 0;
    for(uint8_t value : checked_header) { sum += ~value; }

    // The header is valid if the sum is equal to the checksum
    return sum == checksum;
}



// Pulls the ASCII game title from the ROM header
string Cartridge::parseGameTitle(std::array<uint8_t, 80> header)
{
    std::string title;
    // The game title is stored in ASCII at location $133-$143.
    std::copy(header.begin() + 0x34,
              header.begin() + 0x43 + 1,
              std::back_inserter(title)
    );

    return title;
}



// Writes cartridge information to the log
void Cartridge::dumpCartridge()
{
    log("--BEGIN CART DUMP--", Logger::logDEBUG);
    log(format("ROM File Path: {:s} | SAV File Path: {:s}",
               rom_file_path, sav_file_path),
        Logger::logDEBUG
    );
    log(format("MBC: {:s} | ROM Banks: {:d} | ERAM Banks: {:d} | Persistent ERAM {}",
               mbcToString(mbc), rom_bank_amount, ram_bank_amount, persistent_memory),
        Logger::logDEBUG
    );
    log("--END CART DUMP--", Logger::logDEBUG);
}