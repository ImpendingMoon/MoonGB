// Handles the config file and pref paths

#include "config.hpp"

#define COMPANY "ImpendingMoon"
#define PROGRAM "MoonGB"

using std::string;
using std::array;
using std::cerr;

string pref_path;
int last_win_x, last_win_y;
array<SDL_Color, 5> color_palette;

// Default values
constexpr uint32_t DEF_LAST_WIN_X = 160;
constexpr uint32_t DEF_LAST_WIN_Y = 144;
// Default color palette based off of Gameboy Pocket
constexpr array<SDL_Color, 5> DEF_COLOR_PALETTE = {{
                              {196,207,161,0},    // BG/Off
                              {196,207,161,0},    // Tile0
                              {139,149,109,0},    // Tile1
                              { 77, 83, 60,0},    // Tile2
                              { 31, 31, 31,0} }}; // Tile3

// Attempts to find and load config options from the file
// Loads defaults if file/option not found
void Config::loadConfigFile()
{
    // TODO: Finish loadConfigFile() implementation
    // TEMP: Load defaults always.
    resetAllOptions();
}

// Attempts to save current config options to a file
// Returns -1 if failed
int Config::saveConfigFile()
{
    // TODO: Finish saveConfigFile() implementation
    return -1;
}

// Reset options to default values
void Config::resetAllOptions()
{
    pref_path = SDL_GetPrefPath(COMPANY, PROGRAM);
    last_win_x = DEF_LAST_WIN_X;
    last_win_y = DEF_LAST_WIN_Y;
    color_palette = DEF_COLOR_PALETTE;
}

// Reset a single option to its default value
void Config::resetSingleOption(Options option)
{
    switch(option)
    {
    case PREF_PATH:
    {
        pref_path = SDL_GetPrefPath(COMPANY, PROGRAM);
        break;
    }
    case LAST_WIN_X:
    {
        last_win_x = DEF_LAST_WIN_X;
        break;
    }
    case LAST_WIN_Y:
    {
        last_win_y = DEF_LAST_WIN_Y;
        break;
    }
    case COLOR_PALETTE:
    {
        color_palette = DEF_COLOR_PALETTE;
        break;
    }
    default:
    {
        cerr << "Unhandled option in Config::resetSingleOption()!";
        break;
    }
    }
}

// SGetters
string Config::getPrefPath() { return pref_path; }

int Config::setPrefPath(const std::string& path)
{
    // TODO: Check if path is valid and writable
    pref_path = path;
    return 0;
}

int Config::getLastWinX() { return last_win_x; }

int Config::setLastWinX(int value)
{
    if(value < 0)
    {
        return -1;
    }

    last_win_x = value;
    return 0;
}

int Config::getLastWinY() { return last_win_y; }

int Config::setLastWinY(int value)
{
    if(value < 0)
    {
        return -1;
    }

    last_win_y = value;
    return 0;
}

array<SDL_Color, 5> Config::getColorPalette() { return color_palette; }

int Config::setColorPalette(array<SDL_Color, 5> palette)
{
    color_palette = palette;
    return 0;
}
