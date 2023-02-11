#pragma once

#include "../core.hpp"
#include "gbdefs.hpp"
#include "gameboy.hpp"

class CPU
{
public:
    CPU();
    ~CPU();

    // Pulls the instruction from the program counter, and executes it.
    // Returns the number of cycles used.
    int execute();
private:
    RegisterSet regs;
    FlagRegister flags;
};