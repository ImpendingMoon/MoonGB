// Handles program initialization, loop, and cleanup

#pragma once

#include "../core.hpp"

namespace Program
{
enum ProgramStates
{
    STOPPED, // Program is entirely stopped and outisde of loop
    MENU,    // Emulation is paused/stopped, and input is directed to menus
    RUNNING, // Emulation is running and input is directed to emulator
    EXITING, // Program has been signaled to exit
};

void initProgram();
// NOTE: The program loop is run on the same thread function is called in
void beginProgramLoop();
// Exits all program subcomponents
void quitProgram();

// If the emulator is not running, prompts for a ROM file to start an emu,
// or just switches focus back to the existing emulator
void startEmulator();
// Closes the emulator if running and switches back to the menu.
void quitEmulator();

ProgramStates getProgramState();
void setProgramState(ProgramStates state);

};
