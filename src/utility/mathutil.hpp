// Implements several functions useful for the emulator, but not in stdlib
#pragma once

#include <cinttypes>

namespace Util
{
    // Conversion //

    // Converts two uint8_ts to a uint16_t
    inline uint16_t U8toU16(uint8_t msb, uint16_t lsb)
    {
        return (msb << 8) | lsb;
    }

    // Converts a uint16_t to two uint8_ts
    inline void U16toU8(uint16_t val, uint8_t& msb, uint8_t& lsb)
    {
        msb = (val & 0xFF00) >> 8;
        lsb = val & 0x00FF;
    }

    // Converts an unsigned 8-bit value to a signed 8-bit value
    inline int8_t U8toS8(uint8_t val)
    {
        return static_cast<int8_t>(val);
    }

    // Converts a signed 8-bit value to an unsigned 8-bit value
    inline uint8_t S8toU8(int8_t val)
    {
        return static_cast<uint8_t>(val);
    }

    // Converts an unsigned 16-bit value to a signed 16-bit value
    inline int16_t U16toS16(uint16_t val)
    {
        return static_cast<int16_t>(val);
    }

    // Converts a signed 16-bit value to an unsigned 16-bit value
    inline uint16_t S16toU16(int16_t val)
    {
        return static_cast<uint16_t>(val);
    }

    // Checks //

    // Returns whether an uint8_t addition will overflow
    inline bool checkOFAdd(uint8_t a, uint8_t b)
    {
        return a > (UINT8_MAX - b);
    }

    // Returns whether an uint16_t addition will overflow
    inline bool checkOFAdd(uint16_t a, uint16_t b)
    {
        return a > (UINT16_MAX - b);
    }

    // Returns whether an uint8_t addition will half-carry (lower nibble to upper nibble)
    inline bool checkHCAdd(uint16_t a, uint16_t b)
    {
        return ((a & 0xF) + (b & 0xF)) > 0xF;
    }

    // Returns whether a uint8_t subtraction (a - b) will half-carry (upper nibble to lower nibble)
    inline bool checkHCSub(uint16_t a, uint16_t b)
    {
        return ((a & 0xF) < (b & 0xF));
    }

    // Returns whether an uint8_t subtraction (a - b) will underflow
    inline bool checkUFSub(uint8_t a, uint8_t b)
    {
        return (a < b);
    }

    // Returns whether an uint16_t subtraction (a - b) will underflow
    inline bool checkUFSub(uint16_t a, uint16_t b)
    {
        return (a < b);
    }
}