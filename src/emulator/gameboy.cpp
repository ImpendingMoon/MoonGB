#include "gameboy.hpp"

using Logger::log, std::string;

// Caller should catch std::invalid_argument and std::runtime_exception
Gameboy::Gameboy(const string& _rom_file_path)
{
    log("SYSTEM: Begin loading ROM from: " + _rom_file_path, Logger::logDEBUG);

    rom_file_path = _rom_file_path;
    cart.initCartridge(rom_file_path);
    cart.loadCartridge(mem);
    game_title = cart.getGameTitle();

    log("SYSTEM: Successfully loaded " + game_title, Logger::logDEBUG);
}

Gameboy::~Gameboy() = default;



// Steps the components by one CPU instruction
void Gameboy::step()
{

}



string Gameboy::getRomFilePath() { return rom_file_path; }
string Gameboy::getGameTitle() { return game_title; }


// Dumps emulated system info to the log
void Gameboy::dumpSystem()
{
    cart.dumpCartridge();
    mem.dumpMemory();
}