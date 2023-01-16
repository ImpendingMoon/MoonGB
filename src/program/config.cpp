// Handles the config file and pref paths

#include "config.hpp"

#define COMPANY "ImpendingMoon"
#define PROGRAM "MoonGB"

#include <filesystem>
#include "logger.hpp"

using std::string;
using std::array;
using fmt::format;

using namespace Logger;

bool conf_log_to_stdout, conf_log_to_logfile;
string conf_pref_path;
int conf_last_win_x, conf_last_win_y, conf_log_level;
array<SDL_Color, 5> conf_color_palette;

// Default values
constexpr bool DEF_LOG_TO_STDOUT = true;
constexpr bool DEF_LOG_TO_LOGFILE = true;
constexpr int DEF_LAST_WIN_X = 160;
constexpr int DEF_LAST_WIN_Y = 144;
constexpr int DEF_LOG_LEVEL = 1;
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



void Config::closeConfigFile()
{
    // TODO: Close Config File
}



// Reset options to default values
void Config::resetAllOptions()
{
    conf_log_to_stdout = DEF_LOG_TO_STDOUT;
    conf_log_to_logfile = DEF_LOG_TO_LOGFILE;
    conf_pref_path = SDL_GetPrefPath(COMPANY, PROGRAM);
    conf_last_win_x = DEF_LAST_WIN_X;
    conf_last_win_y = DEF_LAST_WIN_Y;
    conf_log_level = DEF_LOG_LEVEL;
    conf_color_palette = DEF_COLOR_PALETTE;
}



// Reset a single option to its default value
void Config::resetSingleOption(Options option)
{
    switch(option)
    {
    case LOG_TO_STDOUT:
    {
        conf_log_to_stdout = DEF_LOG_TO_STDOUT;
        break;
    }
    case LOG_TO_LOGFILE:
    {
        conf_log_to_logfile = DEF_LOG_TO_LOGFILE;
        break;
    }
    case PREF_PATH:
    {
        conf_pref_path = SDL_GetPrefPath(COMPANY, PROGRAM);
        break;
    }
    case LAST_WIN_X:
    {
        conf_last_win_x = DEF_LAST_WIN_X;
        break;
    }
    case LAST_WIN_Y:
    {
        conf_last_win_y = DEF_LAST_WIN_Y;
        break;
    }
    case LOG_LEVEL:
    {
        conf_log_level = DEF_LOG_LEVEL;
        break;
    }
    case COLOR_PALETTE:
    {
        conf_color_palette = DEF_COLOR_PALETTE;
        break;
    }
    default:
    {
        log("Unhandled option in Config::resetSingleOption()!", ERROR);
        break;
    }
    }
}



// SGetters
string Config::getPrefPath()
{
    return conf_pref_path;
}



int Config::setPrefPath(const std::string& path)
{
    using std::filesystem::exists;
    using std::filesystem::create_directories;
    using std::filesystem::filesystem_error;

    if(!exists(path))
    {
        try {
            create_directories(path);

        } catch(filesystem_error& ex) {
            log(format("ERROR: Cannot set pref path at {:s}!", path), ERROR);
            return -1;
        }
    }

    conf_pref_path = path;
    return 0;
}



int Config::getLastWinX()
{
    return conf_last_win_x;
}



int Config::setLastWinX(int value)
{
    if(value < 0)
    {
        return -1;
    }

    conf_last_win_x = value;
    return 0;
}



int Config::getLastWinY()
{
    return conf_last_win_y;
}



int Config::setLastWinY(int value)
{
    if(value < 0)
    {
        return -1;
    }

    conf_last_win_y = value;
    return 0;
}



bool Config::isLogToStdout()
{
    return conf_log_to_stdout;
}


void Config::setLogToStdout(bool value)
{
    conf_log_to_stdout = value;
}


bool Config::isLogToLogFile()
{
    return conf_log_to_logfile;
}


void Config::setLogToLogFile(bool value)
{
    conf_log_to_logfile = value;
}



int Config::getLogLevel()
{
    return conf_log_level;
}


void Config::setLogLevel(int value)
{
    conf_log_level = value;
}



array<SDL_Color, 5> Config::getColorPalette()
{
    return conf_color_palette;
}



int Config::setColorPalette(array<SDL_Color, 5> palette)
{
    conf_color_palette = palette;
    return 0;
}

