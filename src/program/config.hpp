// Handles the config file and pref paths

#pragma once

#include "../core.hpp"
#include <map>
#include <SDL2/SDL.h>

namespace Config
{

// Attempts to find and load config options from the file
// Loads defaults if file/option not found
void loadConfigFile();
// Attempts to save current config options to a file
// Returns -1 if failed
int saveConfigFile();

void closeConfigFile();

// Reset option(s) to default values
void resetAllOptions();
void resetOption(const std::string& option);

// Gets/sets an option from a key. Callers expected to handle conversion.
std::string getOption(const std::string& option);
void setOption(const std::string& option, const std::string& value);

// Converts a palette of 5 SDL_Colors to a string decodable by stringToPalette()
std::string paletteToString(const std::array<SDL_Color, 5>& palette);
// Converts a string encoded with paletteToString() to a palette of 5 SDL_Colors
std::array<SDL_Color, 5> stringToPalette(const std::string& palette);

};
