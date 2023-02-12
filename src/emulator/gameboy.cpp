#include "gameboy.hpp"

using Logger::log, std::string;

// Caller should catch std::invalid_argument and std::runtime_exception
Gameboy::Gameboy(const string& _rom_file_path)
{
    cycles_per_frame = 70256;
    cycle = 0;

    log("SYSTEM: Begin loading ROM from: " + _rom_file_path, Logger::logDEBUG);

    rom_file_path = _rom_file_path;
    cart.initCartridge(rom_file_path);
    cart.loadCartridge(mem);
    cpu.initCPU(mem);
    game_title = cart.getGameTitle();

    log("SYSTEM: Successfully loaded " + game_title, Logger::logDEBUG);
}

Gameboy::~Gameboy() = default;



// Steps the components by one CPU instruction
void Gameboy::step()
{
    int cycles = cpu.execute(mem);
    cycle += cycles;
}



string Gameboy::getRomFilePath() const { return rom_file_path; }
string Gameboy::getGameTitle() const { return game_title; }
int Gameboy::getCycle() const { return cycle; }
int Gameboy::getCyclesPerFrame() const { return cycles_per_frame; }

void Gameboy::resetCycle()
{
    cycle -= cycles_per_frame;
    if(cycle < 0) { cycle = 0; }
}

// Dumps emulated system info to the log
void Gameboy::dumpSystem()
{
    cpu.dumpCPU();
    cart.dumpCartridge();
    mem.dumpMemory();
}