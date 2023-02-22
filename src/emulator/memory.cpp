#include "memory.hpp"
#include "../program/logger.hpp"
#include <filesystem>

using Logger::log, fmt::format;

Memory::Memory()
{
    // Resize static blocks to correct sizes
    ROM0.data.resize(0x4000);
    VRAM[0].data.resize(0x2000);
    VRAM[1].data.resize(0x2000);
    WRAM0.data.resize(0x1000);
    OAM.data.resize(0x00A0);
    IOReg.data.resize(0x0080);
    HRAM.data.resize(0x007F);
    IEReg.data.resize(0x0001);

    for(size_t i = 0; i < WRAM1.size(); i++)
    {
        WRAM1[i].data.resize(0x1000);
    }
}

Memory::~Memory()
{
    if(p_ERAM.is_open()) { p_ERAM.close(); }
}



// Reads a byte from memory
uint8_t Memory::readByte(uint16_t address)
{
    return readByte(address, false);
}



// Reads a byte from memory, ignoring PPU locks
uint8_t Memory::readByte(uint16_t address, bool ignore_lock)
{
    // Each return value checks if the value is not locked, or if ignore_lock is true
    // If true, return the value, otherwise return 0xFF

    try {
        // ROM0
        if(address >= 0x0000 && address <= 0x3FFF)
        {
            return(!ROM0.is_locked || ignore_lock) ? ROM0.data.at(address) : 0xFF;
        }
        // ROM1
        if(address >= 0x4000 && address <= 0x7FFF)
        {
            auto &mb = ROM1.at(ROM1_index);
            return(!mb.is_locked || ignore_lock) ? mb.data.at(address - 0x4000) : 0xFF;
        }
        // VRAM
        if(address >= 0x8000 && address <= 0x9FFF)
        {
            auto &mb = VRAM.at(VRAM_index);
            return(!mb.is_locked || ignore_lock) ? mb.data.at(address - 0x8000) : 0xFF;
        }
        // ERAM
        if(address >= 0xA000 && address <= 0xBFFF)
        {
            return readERAMByte(ERAM_index, address - 0xA000);
        }
        // WRAM0
        if(address >= 0xC000 && address <= 0xCFFF)
        {
            return(!WRAM0.is_locked || ignore_lock) ? WRAM0.data.at(address - 0xC000) : 0xFF;
        }
        // WRAM1
        if(address >= 0xD000 && address <= 0xDFFF)
        {
            auto &mb = WRAM1.at(WRAM1_index);
            return(!mb.is_locked || ignore_lock) ? mb.data.at(address - 0xD000) : 0xFF;
        }
        // ECHO RAM
        if(address >= 0xE000 && address <= 0xFDFF)
        {
            return readByte(address - 0x2000, ignore_lock);
        }
        // OAM
        if(address >= 0xFE00 && address <= 0xFE9F)
        {
            return (!OAM.is_locked || ignore_lock) ? OAM.data.at(address - 0xFE00) : 0xFF;
        }
        // IO Registers
        if(address >= 0xFF00 && address <= 0xFF7F)
        {
            return(!IOReg.is_locked || ignore_lock) ? IOReg.data.at(address - 0xFF00) : 0xFF;
        }
        // HRAM
        if(address >= 0xFF80 && address <= 0xFFFE)
        {
            return(!HRAM.is_locked || ignore_lock) ? HRAM.data.at(address - 0xFF80) : 0xFF;
        }
        // Interrupt Enable Register
        if(address == 0xFFFF)
        {
            return(!IEReg.is_locked || ignore_lock) ? IEReg.data.at(0) : 0xFF;
        }

    } catch(std::out_of_range& ex) {
        log(format("MEMORY: Invalid switched Memory Bank accessed in readByte! "
                   "Address: ${:04X}. ROM1 Index: {:d}. VRAM Index {:d}. "
                   "ERAM Index: {:d}. WRAM1 Index: {:d}. Exception: {:s}",
                   address, ROM1_index, VRAM_index, ERAM_index, WRAM1_index,
                   ex.what()),
            Logger::logDEBUG
        );

        return 0xFF;
    }

    log(format("MEMORY: Invalid address provided to readByte! Address: ${:04X}",
               address),
        Logger::logDEBUG
    );

    return 0xFF;
}



// Reads a byte from memory, does not log. For dumping
uint8_t Memory::getByte(uint16_t address)
{
    // Each return value checks if the value is not locked, or if ignore_lock is true
    // If true, return the value, otherwise return 0xFF

    try {
        // ROM0
        if(address >= 0x0000 && address <= 0x3FFF)
        {
            return ROM0.data.at(address);
        }
        // ROM1
        if(address >= 0x4000 && address <= 0x7FFF)
        {
            auto &mb = ROM1.at(ROM1_index);
            return mb.data.at(address - 0x4000);
        }
        // VRAM
        if(address >= 0x8000 && address <= 0x9FFF)
        {
            auto &mb = VRAM.at(VRAM_index);
            return mb.data.at(address - 0x8000);
        }
        // ERAM
        if(address >= 0xA000 && address <= 0xBFFF)
        {
            return readERAMByte(ERAM_index, address - 0xA000);
        }
        // WRAM0
        if(address >= 0xC000 && address <= 0xCFFF)
        {
            return WRAM0.data.at(address - 0xC000);
        }
        // WRAM1
        if(address >= 0xD000 && address <= 0xDFFF)
        {
            auto &mb = WRAM1.at(WRAM1_index);
            return mb.data.at(address - 0xD000);
        }
        // ECHO RAM
        if(address >= 0xE000 && address <= 0xFDFF)
        {
            return getByte(address - 0x2000);
        }
        // OAM
        if(address >= 0xFE00 && address <= 0xFE9F)
        {
            return OAM.data.at(address - 0xFE00);
        }
        // IO Registers
        if(address >= 0xFF00 && address <= 0xFF7F)
        {
            return IOReg.data.at(address - 0xFF00);
        }
        // HRAM
        if(address >= 0xFF80 && address <= 0xFFFE)
        {
            return HRAM.data.at(address - 0xFF80);
        }
        // Interrupt Enable Register
        if(address == 0xFFFF)
        {
            return IEReg.data.at(0);
        }

    } catch(std::out_of_range& ex) {
        return 0x00;
    }

    return 0x00;
}



// Writes a byte to memory
void Memory::writeByte(uint16_t address, uint8_t data)
{
    try {
        // ROM0
        if(address >= 0x0000 && address <= 0x3FFF)
        {
            log(format("MEMORY: Attempted write to ROM! Address: ${:04X}", address),
                Logger::logDEBUG);
            return;
        }
        // ROM1
        if(address >= 0x4000 && address <= 0x7FFF)
        {
            log(format("MEMORY: Attempted write to ROM! Address: ${:04X}", address),
                Logger::logDEBUG);
            return;
        }
        // VRAM
        if(address >= 0x8000 && address <= 0x9FFF)
        {
            auto &mb = VRAM.at(VRAM_index);
            if (!mb.is_locked) { mb.data.at(address - 0x8000) = data; }
            return;
        }
        // ERAM
        if(address >= 0xA000 && address <= 0xBFFF)
        {
            writeERAMByte(ERAM_index, address - 0xA000, data);
            return;
        }
        // WRAM0
        if(address >= 0xC000 && address <= 0xCFFF)
        {
            if(!WRAM0.is_locked) { WRAM0.data.at(address - 0xC000) = data; }
            return;
        }
        // WRAM1
        if(address >= 0xD000 && address <= 0xDFFF)
        {
            auto &mb = WRAM1.at(WRAM1_index);
            if(!mb.is_locked) { mb.data.at(address - 0xD000) = data; }
            return;
        }
        // ECHO RAM
        if(address >= 0xE000 && address <= 0xFDFF)
        {
            writeByte(address - 0x2000, data);
            return;
        }
        // OAM
        if(address >= 0xFE00 && address <= 0xFE9F)
        {
            if(!OAM.is_locked) { OAM.data.at(address - 0xFE00) = data; }
            return;
        }
        // IO Registers
        if(address >= 0xFF00 && address <= 0xFF7F)
        {
            if (!IOReg.is_locked) { IOReg.data.at(address - 0xFF00) = data; }
            return;
        }
        // HRAM
        if(address >= 0xFF80 && address <= 0xFFFE)
        {
            if(!HRAM.is_locked) { HRAM.data.at(address - 0xFF80) = data; }
            return;
        }
        // Interrupt Enable Register
        if(address == 0xFFFF)
        {
            if (!IEReg.is_locked) { IEReg.data.at(0) = data; }
            return;
        }

    } catch(std::out_of_range& ex) {
        log(format("MEMORY: Invalid switched Memory Bank accessed in writeByte! "
                   "Address: ${:04X}. ROM1 Index: {:d}. VRAM Index {:d}. "
                   "ERAM Index: {:d}. WRAM1 Index: {:d}. Exception: {:s}",
                   address, ROM1_index, VRAM_index, ERAM_index, WRAM1_index,
                   ex.what()),
            Logger::logDEBUG
        );

        return;
    }

    log(format("MEMORY: Invalid address provided to writeByte! Address: ${:04X}",
               address),
        Logger::logDEBUG
    );
}



// Copies an array of 0x4000 bytes into ROM0
void Memory::loadROM0(const std::array<uint8_t, 0x4000>& data)
{
    std::copy(data.begin(), data.end(), ROM0.data.begin());
}


// Copies a vector of MemoryBanks into ROM1
void Memory::loadROM1(const std::vector<MemoryBank>& data, uint16_t bank_amount)
{
    ROM_bank_amount = bank_amount;
    ROM1 = data;
}


// Sets the currently selected ROM1 bank
void Memory::setROM1Index(const uint8_t& index)
{
    ROM1_index = index;
}


// Sets the currently selected ERAM bank
void Memory::setERAM(const uint16_t& _bank_amount,
             bool _persistent,
             const std::string& _sav_file_path,
             BankController _mbc)
{
    ERAM_bank_amount = _bank_amount;
    ERAM_persistent = _persistent;
    mbc = _mbc;
    sav_file_path = _sav_file_path;

    if(ERAM_persistent)
    {
        // Open as out-only, to make sure the file is created
        std::ofstream temp(_sav_file_path, std::fstream::out);
        temp.close();

        p_ERAM.open(_sav_file_path, std::fstream::out
                                    | std::fstream::in
                                    | std::fstream::binary);

        if(!p_ERAM)
        {
            log("MEMORY: Could not open .sav file! Saves will not be permanent!",
                Logger::logERROR);
        }

        // Ensure that the .sav file is the correct size
        size_t target_size = ERAM_bank_amount * 0x2000;
        if(p_ERAM && std::filesystem::file_size(_sav_file_path) != target_size)
        {
            std::filesystem::resize_file(_sav_file_path, target_size);
        }
    }

    if(!ERAM_persistent || !p_ERAM)
    {
        np_ERAM.resize(ERAM_bank_amount * 0x2000);
    }
}



// Sets locks for PPU
void Memory::setVRAMLock(bool value)
{
    for(auto bank : VRAM)
    {
        bank.is_locked = value;
    }
}

void Memory::setOAMLock(bool value)
{
    OAM.is_locked = value;
}


// Dumps the contents of memory to the log
void Memory::dumpMemory()
{
    std::string output{};

    log("--BEGIN MEMORY DUMP--", Logger::logDEBUG);

    // Iterate through entire address space
    for(int i = 0; i <= 0xFFFF; i++)
    {
        // Add newline and address every 32 bytes
        if(i % 32 == 0)
        {
            // Don't log blank line
            if(i != 0)
            {
                // Log and clear string every newline
                log(output, Logger::logDEBUG);
                output = "";
            }

            // Add address label
            output.append( fmt::format("${:04X} ", i) );
        }

        uint8_t value = getByte(i);
        output.append( fmt::format("{:02X} ", value) );
    }

    log("--END MEMORY DUMP--", Logger::logDEBUG);
}




uint8_t Memory::readERAMByte(uint8_t bank, uint16_t address)
{
    if(bank > ERAM_bank_amount)
    {
        log(format("MEMORY: Attempted read of invalid ERAM bank. "
                   "Requested Bank: {:d} | Bank Amount: {:d}",
                   bank, ERAM_bank_amount),
            Logger::logDEBUG);
    }

    if(ERAM_persistent && p_ERAM)
    {
        p_ERAM.seekg(bank * address);
        return p_ERAM.get();
    }

    return np_ERAM.at(bank * address);
}




void Memory::writeERAMByte(uint8_t bank, uint16_t address, uint8_t data)
{
    if(bank > ERAM_bank_amount)
    {
        log(format("MEMORY: Attempted write to invalid ERAM bank. "
                   "Requested Bank: {:d} | Bank Amount: {:d}",
                   bank, ERAM_bank_amount),
            Logger::logDEBUG);
    }

    if(ERAM_persistent && p_ERAM)
    {
        p_ERAM.seekg(bank * address);
        p_ERAM.put(data);
        return;
    }

    np_ERAM.at(bank * address) = data;
}
