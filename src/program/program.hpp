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

ProgramStates getProgramState();
void setProgramState(ProgramStates state);

};
