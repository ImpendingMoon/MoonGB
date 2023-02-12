#pragma once

#include "../core.hpp"
#include "gbdefs.hpp"
#include "memory.hpp"
#include "../program/program.hpp"

class CPU
{
public:
    CPU();
    ~CPU();

    // Initializes the CPU registers, using memory to fake the post-BIOS state
    void initCPU(Memory& mem);

    // Pulls the instruction from the program counter, and executes it.
    // Returns the number of cycles used.
    int execute(Memory& mem);

    // Gets a byte from an 8-bit register
    inline uint8_t getByteReg(TargetID target) const;
    // Sets an 8-bit register to a value
    inline void setByteReg(TargetID target, uint8_t value);
    // Gets a short from a 16-bit register
    inline uint16_t getShortReg(TargetID target) const;
    // Sets a 16-bit register to a value
    inline void setShortReg(TargetID target, uint16_t value);

    // Logs CPU information
    void dumpCPU();

private:
    RegisterSet regs{};
    FlagRegister flags{};

    Instruction lastInstruction;

    bool halted = false;
    bool interrupts_enabled = false;
    bool next_interrupt_state = false;

    // Converts a 3-bit ID to a TargetID
    inline static TargetID toTarget(uint8_t id);
    // Converts a BIT target to a TargetID
    inline static TargetID toBITTarget(uint8_t bit);
};



TargetID CPU::toTarget(uint8_t id)
{
    // 111 = RegA // 000 = RegB // 001 = RegC // 010 = RegD
    // 011 = RegE // 100 = RegH // 101 = RegL // 110 = (HL)
    switch (id & 0b00000111)
    {
        case 0b111: return A;
        case 0b000: return B;
        case 0b001: return C;
        case 0b010: return D;
        case 0b011: return E;
        case 0b100: return H;
        case 0b101: return L;
        case 0b110: return HL;
    }
    return NOTARGET; // Should be impossible, but just in case.
}

TargetID CPU::toBITTarget(uint8_t bit)
{
    switch(bit & 0b00000111)
    {
        case 0b000: return BIT0;
        case 0b001: return BIT1;
        case 0b010: return BIT2;
        case 0b011: return BIT3;
        case 0b100: return BIT4;
        case 0b101: return BIT5;
        case 0b110: return BIT6;
        case 0b111: return BIT7;
    }
    return NOTARGET; // Should be impossible, but just in case
}

// Gets a byte from an 8-bit register
uint8_t CPU::getByteReg(TargetID target) const
{
    switch(target)
    {
    case A: return regs.a;
    case F: return regs.f;
    case B: return regs.b;
    case C: return regs.c;
    case D: return regs.d;
    case E: return regs.e;
    case H: return regs.h;
    case L: return regs.l;
    default:
    {
        throw std::invalid_argument(
            "Attempted read of invalid target in getByteReg()"
        );
    }
    }
}

// Sets an 8-bit register to a value
void CPU::setByteReg(TargetID target, uint8_t value)
{
    switch(target)
    {
    case A: regs.a = value; return;
    case F: regs.f = value; return;
    case B: regs.b = value; return;
    case C: regs.c = value; return;
    case D: regs.d = value; return;
    case E: regs.e = value; return;
    case H: regs.h = value; return;
    case L: regs.l = value; return;
    default:
    {
        throw std::invalid_argument(
            "Attempted write of invalid target in setByteReg()"
        );
    }
    }
}

// Gets a short from a 16-bit register
uint16_t CPU::getShortReg(TargetID target) const
{
    switch(target)
    {
    case AF: return Util::U8toU16(regs.a, regs.f);
    case BC: return Util::U8toU16(regs.b, regs.c);
    case DE: return Util::U8toU16(regs.d, regs.e);
    case HL: return Util::U8toU16(regs.h, regs.l);
    case SP: return regs.sp;
    case PC: return regs.pc;
    default:
    {
        throw std::invalid_argument(
                "Attempted read of invalid target in getShortReg()"
        );
    }
    }
}

// Sets a 16-bit register to a value
void CPU::setShortReg(TargetID target, uint16_t value)
{
    switch(target)
    {
    case AF: Util::U16toU8(value, regs.a, regs.f); break;
    case BC: Util::U16toU8(value, regs.b, regs.c); break;
    case DE: Util::U16toU8(value, regs.d, regs.e); break;
    case HL: Util::U16toU8(value, regs.h, regs.l); break;
    case SP: regs.sp = value; break;
    case PC: regs.pc = value; break;
    default:
    {
        throw std::invalid_argument(
            "Attempted write of invalid target in setShortReg()"
        );
    }
    }
}