# MoonGB

- Yet another Gameboy Emulator!

This may or may not be my fourth attempt at writing a Gameboy emulator, but shh...

MoonGB is mostly being made as a way to learn C++, SDL, and system architecture. I also want an excuse to play Pokemon, because just downloading an emulator was "too easy".

At the moment, I'm primarily focusing on getting a somewhat decent user experience with SDL before jumping into the main emulator again, as that's what I need the most practice in.

## Project Goals:

1) Have a working Gameboy emulator (duh)
2) Have a decent GUI
3) Support most memory mappers and persistent saves
4) Support color palettes
5) Support remapping keys
6) Implement Gameboy Color compatibility

## Dependencies:
1) SDL2 - Version 2.26 or higher
2) {fmt} - Version 9.0.0 or higher

## Building:
1) Clone repo into a new folder
2) Ensure all dependencies are installed
3) Create a separate build directory
4) Run cmake --DCMAKE_BUILD_TYPE="Release" <path_to_repo_directory>
5) Run Make/Ninja
6) Enjoy
