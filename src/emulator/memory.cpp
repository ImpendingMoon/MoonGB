#include "memory.hpp"
#include "../program/logger.hpp"

using Logger::log, fmt::format;

Memory::Memory()
{
    // Resize static blocks to correct sizes
    ROM0.data.resize(0x4000);
    WRAM0.data.resize(0x1000);
    OAM.data.resize(0x00A0);
    IOReg.data.resize(0x0080);
    HRAM.data.resize(0x007F);
    IEReg.data.resize(0x0001);
}

Memory::~Memory() = default;



// Reads a byte from memory
uint8_t Memory::readByte(uint16_t address)
{
    readByte(address, false);
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
            // TODO: Implement when Cartridge is implemented
            return 0xFF;
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
            // TODO: Implement when Cartridge is implemented
            return 0x00;
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
            if(!ROM0.is_locked) { ROM0.data.at(address) = data; }
            return;
        }
        // ROM1
        if(address >= 0x4000 && address <= 0x7FFF)
        {
            auto &mb = ROM1.at(ROM1_index);
            if(!mb.is_locked) { mb.data.at(address - 0x4000) = data; }
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
            // TODO: Implement when Cartridge is implemented
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
