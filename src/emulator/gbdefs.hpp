// Defines various information and structs for the emulator core
#pragma once

#include "../core.hpp"
#include "../utility/mathutil.hpp"
#include "../program/logger.hpp"

static constexpr uint32_t GB_X_RES = 160;
static constexpr uint32_t GB_Y_RES = 144;

// Previous implementations used anonymous structs to implicitly define combined
// regs, which was fine in C, but is undefined in C++.
// Although it usually works, compiler warnings are annoying.
struct RegisterSet
{
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t sp;
    uint16_t pc;
};

// Targets used by the CPU for decode/execute
enum TargetID
{
    NOTARGET,
    A, F, B, C, D, E, H, L,
    AF, BC, DE, HL,
    SP, PC,
    IMMEDIATE,
    BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
};

// Human-readable representation of the F register
class FlagRegister
{
public:
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;

    static constexpr uint32_t ZERO_POS = 7;
    static constexpr uint32_t SUBTRACT_POS = 6;
    static constexpr uint32_t HALF_CARRY_POS = 5;
    static constexpr uint32_t CARRY_POS = 4;

    inline void byteToFlags(uint8_t f_reg)
    {
        zero = (f_reg >> ZERO_POS) & 1;
        subtract = (f_reg >> SUBTRACT_POS) & 1;
        half_carry = (f_reg >> HALF_CARRY_POS) & 1;
        carry = (f_reg >> CARRY_POS) & 1;
    }

    inline uint8_t flagsToByte() const
    {
        uint8_t f_reg = zero << ZERO_POS;
        f_reg |= subtract << SUBTRACT_POS;
        f_reg |= half_carry << HALF_CARRY_POS;
        f_reg |= carry << CARRY_POS;
        return f_reg;
    }
};

// Used to indicate MBC controls and memory persistence
enum BankController
{
    // Each token set to actual ID value

    NONE = 0x00, // No additional banks.
    NONE_RAM = 0x08, // RAM
    NONE_BAT_RAM = 0x09, // Battery RAM

    MBC1 = 0x01, // Up to 2MiB ROM, 32KiB RAM
    MBC1_RAM = 0x02, // MBC1 with RAM
    MBC1_BAT_RAM = 0x03, // MBC1 with Battery RAM

    MBC2 = 0x05, // Up to 256KiB ROM, 256 bytes of mapped memory(?)
    MBC2_BAT = 0x06, //

    MBC3 = 0x11, // Up to 2MiB ROM, 32KiB RAM, Additional Timer
    MBC3_RAM = 0x12, // MBC3 with RAM
    MBC3_BAT_RAM = 0x13, // MBC3 with Battery RAM
    MBC3_BAT_TIMER = 0x0F, // MBC3 with Battery + Timer
    MBC3_BAT_RAM_TIMER = 0x10, // MBC3 with Battery + RAM + Timer

    // MBC4 does not exist.

    MBC5 = 0x19, // Up to 8MiB ROM
    MBC5_RAM = 0x1A, // MBC5 with RAM
    MBC5_BAT_RAM = 0x1B, // MBC5 with Battery RAM
    MBC5_RUMBLE = 0x1C, // MBC5 with Rumble (rumble not emulated)
    MBC5_RUMBLE_RAM = 0x1D, // MBC5 with Rumble and RAM
    MBC5_RUMBLE_BAT_RAM = 0x1E, // MBC5 with Rumble, Battery, and RAM

    MBC6 = 0x20, // Used in one game that can't be emulated anyway

    // MBC7 is used for an accelerometer, which I don't plan on emulating.
    // Will not be emulating MMM or M161 cartridges

    HuC1 = 0xFF, // Seems easy enough to emulate, if I ever find the documentation.
    HuC3 = 0xFE, // Alongside banking, contains an RTC, speaker, and IR Blaster/Receiver. Fancy.
    // Not emulating Wisdom Tree MBC
    // Not emulating BANDAI TAMA5
    // Probably not emulating GB Camera
};

// The Gameboy's memory is split among several banks
struct MemoryBank
{
    std::vector<uint8_t> data;
    bool is_locked;
};

// Used for logging.
struct Instruction
{
    std::string mnemonic;
    TargetID target1;
    TargetID target2;
    bool t1_as_address;
    bool t2_as_address;
    uint16_t origin;
    uint16_t opcode;
    bool two_byte;
};

// targetToString stuff
inline std::string targetToString(TargetID target)
{
    switch(target)
    {
        case NOTARGET: return "NOTARGET";
        case A: return "A";
        case F: return "F";
        case B: return "B";
        case C: return "C";
        case D: return "D";
        case E: return "E";
        case H: return "H";
        case L: return "L";
        case AF: return "AF";
        case BC: return "BC";
        case DE: return "DE";
        case HL: return "HL";
        case SP: return "SP";
        case PC: return "PC";
        case BIT0: return "0";
        case BIT1: return "1";
        case BIT2: return "2";
        case BIT3: return "3";
        case BIT4: return "4";
        case BIT5: return "5";
        case BIT6: return "6";
        case BIT7: return "7";
        case IMMEDIATE: return "IMMEDIATE";
    }
    return "ERROR";
}

inline std::string mbcToString(BankController mbc)
{
    switch(mbc)
    {
        case NONE: return "NONE";
        case NONE_RAM: return "NONE_RAM";
        case NONE_BAT_RAM: return "NONE_BAT_RAM";
        case MBC1: return "MBC1";
        case MBC1_RAM: return "MBC1_RAM";
        case MBC1_BAT_RAM: return "MBC1_BAT_RAM";
        case MBC2: return "MBC2";
        case MBC2_BAT: return "MBC2_BAT";
        case MBC3: return "MBC3";
        case MBC3_RAM: return "MBC3_RAM";
        case MBC3_BAT_RAM: return "MBC3_BAT_RAM";
        case MBC3_BAT_TIMER: return "MBC3_BAT_TIMER";
        case MBC3_BAT_RAM_TIMER: return "MBC3_BAT_RAM_TIMER";
        case MBC5: return "MBC5";
        case MBC5_RAM: return "MBC5_RAM";
        case MBC5_BAT_RAM: return "MBC5_BAT_RAM";
        case MBC5_RUMBLE: return "MBC5_RUMBLE";
        case MBC5_RUMBLE_RAM: return "MBC5_RUMBLE_RAM";
        case MBC5_RUMBLE_BAT_RAM: return "MBC5_RUMBLE_BAT_RAM";
        case MBC6: return "MBC6";
        case HuC1: return "HuC1";
        case HuC3: return "HuC3";
    }
    return "ERROR";
}

inline std::string insToString(Instruction inst)
{
    using std::string, fmt::format;

    string t1;
    if(inst.target1 != NOTARGET)
    {
        if(inst.target1 == IMMEDIATE)
        {
            t1 = "n";
        } else {
            if(inst.t1_as_address)
            {
                t1 = format("[{}]", targetToString(inst.target1));
            } else {
                t1 = targetToString(inst.target1);
            }
        }
    }

    string t2;
    if(inst.target2 != NOTARGET)
    {
        if(inst.target2 == IMMEDIATE)
        {
            t2 = ", n";
        } else {
            if(inst.t2_as_address)
            {
                t2 = format(", [{}]", targetToString(inst.target2));
            } else {
                t2 = ", " + targetToString(inst.target2);
            }
        }
    }

    // Format in Intel-like assembly syntax
    return format("{} {}{} | Origin ${:04X} | Opcode 0x{:02X}",
                  inst.mnemonic, t1, t2, inst.origin, inst.opcode);
}

inline std::string regsToString(RegisterSet regs)
{
    std::string output = "";
    output.append("(");
    output.append(fmt::format("A: 0x{:02X} | ", regs.a));
    output.append(fmt::format("F: 0b{:08b} | ", regs.f));
    output.append(fmt::format("B: 0x{:02X} | ", regs.b));
    output.append(fmt::format("C: 0x{:02X} | ", regs.c));
    output.append(fmt::format("D: 0x{:02X} | ", regs.d));
    output.append(fmt::format("E: 0x{:02X} | ", regs.e));
    output.append(fmt::format("H: 0x{:02X} | ", regs.h));
    output.append(fmt::format("L: 0x{:02X} | ", regs.l));
    output.append(fmt::format("SP: 0x{:04X} | ", regs.sp));
    output.append(fmt::format("PC: 0x{:04X}", regs.pc));
    output.append(")");
    return output;
}
