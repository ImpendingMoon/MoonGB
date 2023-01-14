# MoonGB

#### Yet another Gameboy Emulator!

This may or may not be my fourth attempt at writing a Gameboy emulator, but shh...

MoonGB is mostly being made as a way to learn C++, SDL, and system architecture. I also want an excuse to play Pokemon, because just downloading an emulator was "too easy".

At the moment, I'm primarily focusing on getting a somewhat decent user experience with SDL before jumping into the main emulator again, as that's what I need the most practice in.

## Project Goals:

1) Have a working Gameboy emulator (duh)
2) Have a decent GUI
3) Support most memory mappers and persistent saves
4) Implement most memory mappers
5) Support color palettes
6) Support remapping keys
7) Implement Gameboy Color compatibility

## Building
MoonGB is primarily developed on Linux, and although I will try to include Windows support where needed, there is no guarantee that it will run on Windows.

#### Dependencies:
1) SDL2 - Version 2.26 or higher
2) SDL2_ttf - Version 2.20 or higher
3) {fmt} - Version 9.0.0 or higher
