// Handles the config file and pref paths

#include "config.hpp"

#define COMPANY "ImpendingMoon"
#define PROGRAM "MoonGB"

#include <filesystem>
#include <sstream>
#include <algorithm>
#include "logger.hpp"

using std::string;
using std::map;
using std::array;
using fmt::format;

using Logger::log;

map<string, string> options{
    {"PrefPath", "."},
    {"LogLevel", "3"},
    {"LogToStdout", "1"},
    {"LogToLogFile", "1"},
    {"WinSizeX", "160"},
    {"WinSizeY", "144"},
    // Default color palette based off of Gameboy Pocket
    {"ColorPalette", "{200,211,165,000} " // BG
                     "{196,207,161,000} " // Tile0
                     "{139,149,109,000} " // Tile1
                     "{077,083,060,000} " // Tile2
                     "{031,031,031,000} " // Tile3
    },
};

// Defaults
const map<string, string> DEF_OPTIONS {
    {"PrefPath", "."},
    {"LogLevel", "3"},
    {"LogToStdout", "1"},
    {"LogToLogFile", "1"},
    {"WinSizeX", "160"},
    {"WinSizeY", "144"},
    // Default color palette based off of Gameboy Pocket
    {"ColorPalette", "{200,211,165,000} " // BG
                     "{196,207,161,000} " // Tile0
                     "{139,149,109,000} " // Tile1
                     "{077,083,060,000} " // Tile2
                     "{031,031,031,000} " // Tile3
    },
};



// Attempts to find and load config options from the file
// Loads defaults if file/option not found
void Config::loadConfigFile()
{
    /*
     * PLANNED IMPLEMENTATION:
     *
     * 1. Look for MoonGB.ini in '.' and 'SDL_GetPrefPath()'
     * 2. If not found, log and stop checking. Defaults already initialized.
     * 3. If found, loop through every line and parse key, value pair,
     *    using setOption() for error handling
     */

    log("CONFIG: loadConfigFile() not implemented! Loading defaults...",
        Logger::ERROR);
}

int Config::saveConfigFile()
{
    /*
     * PLANNED IMPLEMENTATION:
     *
     * 1. Look to see if MoonGB.ini is already loaded
     * 2. If not, look for MoonGB.ini in '.' and 'SDL_GetPrefPath()'
     * 3. If not found, try creating MoonGB.ini in '.'
     * 4. If failed, try creating MoonGB.ini in 'SDL_GetPrefPath()'
     * 5. If failed, abort.
     * 6. Write "[default]" to top line to comply with .ini spec
     * 7. Iterate through all options, in format:
     *    "key"="value"
     s    one pair per line.
     */

    log("CONFIG: saveConfigFile() not implemented! Aborting...",
        Logger::ERROR);
    return -1;
}

void Config::closeConfigFile()
{

}


// Reset option(s) to default values
void Config::resetAllOptions()
{
    for(const auto& [key, value] : options)
    {
        options[key] = DEF_OPTIONS.at(key);
    }
}

void Config::resetOption(string option)
{
    try {
        options.at(option) = DEF_OPTIONS.at(option);

    } catch(std::out_of_range& ex) {
        log(format("CONFIG: Invalid option {:s} passed to resetOption(): {:s}", option, ex.what()),
        Logger::ERROR);
    }
}


// Gets/sets an option from a key. Callers expected to handle conversion.
string Config::getOption(string option)
{
    try {
        return options.at(option);

    } catch(std::out_of_range& ex) {
        log(format("CONFIG: Invalid option {:s} passed to getOption()", option),
            Logger::ERROR);
        return "";
    }
}

void Config::setOption(string option, string value)
{
    try {
        options.at(option) = value;

    } catch(std::out_of_range& ex) {
        log(format("CONFIG: Invalid option {:s} passed to getOption()", option),
            Logger::ERROR);
    }
}



// Converts a palette of 5 SDL_Colors to a string decodable by stringToPalette()
string Config::paletteToString(array<SDL_Color, 5> palette)
{
    string output{};

    for(SDL_Color color : palette)
    {
        output.append("{");
        output.append(format("{:03d},{:03d},{:03d},{:03d}",
                              color.r, color.g, color.b, color.a));
        output.append("} ");
    }

    return output;
}



// Converts a string encoded with paletteToString() to a palette of 5 SDL_Colors
array<SDL_Color, 5> Config::stringToPalette(string palette)
{
    using std::stringstream;

    array<SDL_Color, 5> output{};

    // Simple error detection by counting number of chars in string compared
    // to the known-good default palette
    // This is easy to defeat, but should be good enough for accidents.
    if(palette.length() != DEF_OPTIONS.at("ColorPalette").length())
    {
        // Not try/catching because there will always be "ColorPalette" key
        palette = DEF_OPTIONS.at("ColorPalette");
        log("CONFIG: Malformed color palette given to stringToPalette()."
            "Loading defaults...", Logger::ERROR);
    }

    // Get values from string
    char ignore;
    stringstream str(palette);
    for(size_t i = 0; i < output.size(); i++)
    {
        // Stringstream junk to ignore formatting
        str >> ignore >> output[i].r >> ignore >> output[i].g >>
               output[i].b >> ignore >> output[i].a >> ignore;
    }

    return output;
}

