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

// toString stuff
std::string toString(TargetID target)
{
    std::string output = "";
    switch(target)
    {
        case NOTARGET: output = "NOTARGET"; break;
        case A: output = "A"; break;
        case F: output = "F"; break;
        case B: output = "B"; break;
        case C: output = "C"; break;
        case D: output = "D"; break;
        case E: output = "E"; break;
        case H: output = "H"; break;
        case L: output = "L"; break;
        case AF: output = "AF"; break;
        case BC: output = "BC"; break;
        case DE: output = "DE"; break;
        case HL: output = "HL"; break;
        case SP: output = "SP"; break;
        case PC: output = "PC"; break;
        case IMMEDIATE: output = "IMMEDIATE"; break;
    }
    return output;
}

std::string toString(BankController mbc)
{
    std::string output = "";
    switch(mbc)
    {
        case NONE: output = "NONE"; break;
        case NONE_RAM: output = "NONE_RAM"; break;
        case NONE_BAT_RAM: output = "NONE_BAT_RAM"; break;
        case MBC1: output = "MBC1"; break;
        case MBC1_RAM: output = "MBC1_RAM"; break;
        case MBC1_BAT_RAM: output = "MBC1_BAT_RAM"; break;
        case MBC2: output = "MBC2"; break;
        case MBC2_BAT: output = "MBC2_BAT"; break;
        case MBC3: output = "MBC3"; break;
        case MBC3_RAM: output = "MBC3_RAM"; break;
        case MBC3_BAT_RAM: output = "MBC3_BAT_RAM"; break;
        case MBC3_BAT_TIMER: output = "MBC3_BAT_TIMER"; break;
        case MBC3_BAT_RAM_TIMER: output = "MBC3_BAT_RAM_TIMER"; break;
        case MBC5: output = "MBC5"; break;
        case MBC5_RAM: output = "MBC5_RAM"; break;
        case MBC5_BAT_RAM: output = "MBC5_BAT_RAM"; break;
        case MBC5_RUMBLE: output = "MBC5_RUMBLE"; break;
        case MBC5_RUMBLE_RAM: output = "MBC5_RUMBLE_RAM"; break;
        case MBC5_RUMBLE_BAT_RAM: output = "MBC5_RUMBLE_BAT_RAM"; break;
        case MBC6: output = "MBC6"; break;
        case HuC1: output = "HuC1"; break;
        case HuC3: output = "HuC3"; break;
    }
    return output;
}

std::string toString(Instruction ins)
{
    // String formatting my beloved...
    std::string output = "";
    std::string t1str = "", t2str = "";

    output.append("(");
    if(ins.target1 == NOTARGET) { t1str = ""; }
    else if(ins.target1 == IMMEDIATE) { t1str = "n"; }
    else { t1str = toString(ins.target1); }

    if(ins.target2 == NOTARGET) { t2str = ""; }
    else if(ins.target2 == IMMEDIATE) { t2str = "n"; }
    else { t2str = toString(ins.target2); }

    output.append(fmt::format("INS: {:s} ", ins.mnemonic));

    if(ins.t1_as_address)
    {
        output.append(fmt::format("({:s})", t1str));
    } else {
        output.append(fmt::format("{:s}", t1str));
    }

    if(ins.t2_as_address)
    {
        output.append(fmt::format("({:s})", t2str));
    } else {
        output.append(fmt::format("{:s}", t2str));
    }

    output.append(" | ");

    output.append(fmt::format("Origin Address: ${:04X} | ", ins.origin));

    if(ins.two_byte)
    {
        output.append(fmt::format("Opcode: 0x{:04X}", ins.opcode));
    } else {
        output.append(fmt::format("Opcode: 0x{:02X}", ins.opcode));
    }
    output.append(")");

    return output;
}

std::string toString(RegisterSet regs)
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
