// Handles the config file and pref paths

#pragma once

#include "../core.hpp"
#include <SDL2/SDL.h>

namespace Config
{

// NOTE: A dictionary might be better for this.
enum Options
{
    // Boolean
    LOG_TO_STDOUT,
    LOG_TO_LOGFILE,
    // String
    PREF_PATH,
    // Numeric
    LAST_WIN_X,
    LAST_WIN_Y,
    LOG_LEVEL,
    // Misc.
    COLOR_PALETTE
};

// Attempts to find and load config options from the file
// Loads defaults if file/option not found
void loadConfigFile();
// Attempts to save current config options to a file
// Returns -1 if failed
int saveConfigFile();

void closeConfigFile();

// Reset option(s) to default values
void resetAllOptions();
void resetSingleOption(Options option);

// SGetters
std::string getPrefPath();
int setPrefPath(const std::string& path);

int getLastWinX();
int setLastWinX(int value);
int getLastWinY();
int setLastWinY(int value);

bool isLogToStdout();
void setLogToStdout(bool value);
bool isLogToLogFile();
void setLogToLogFile(bool value);

int getLogLevel();
void setLogLevel(int value);

// Returns an array of four colors used for the color palette
// In ascending order from BG to Tile0-Tile3
std::array<SDL_Color, 5> getColorPalette();
// Sets the current color palette to an array of SDL_Colors
// In ascending order from BG to Tile0-Tile3
// Returns -1 if failed, 0 otherwise
int setColorPalette(std::array<SDL_Color, 5> palette);

};
