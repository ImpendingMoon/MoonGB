// Handles the config file and pref paths

#include "config.hpp"

#define COMPANY "ImpendingMoon"
#define PROGRAM "MoonGB"

#include <filesystem>
#include <algorithm>
#include "logger.hpp"

using std::string, std::map, std::array, fmt::format, Logger::log;

map<string, string> options{};

// Defaults
const map<string, string> DEF_OPTIONS {
    {"PrefPath", "./"},
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
    resetAllOptions();
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
    for(const auto& [key, value] : DEF_OPTIONS)
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
    using std::stoi;

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
    for(size_t i = 0; i < output.size(); i++)
    {
        // Since each color is formatted at the same size, substrings can be
        // used to get the values for stoi.
        // I feel like I should have just stored colors in a solid block of ints

        // String offset position. Each color is 18 characters long
        int offset = i * 18;

        string r,g,b,a;
        // Base offset + Char offset, Length
        r = palette.substr(offset + 1, 3);
        g = palette.substr(offset + 5, 3);
        b = palette.substr(offset + 9, 3);
        a = palette.substr(offset + 13, 3);

        output[i].r = stoi(r);
        output[i].g = stoi(g);
        output[i].b = stoi(b);
        output[i].a = stoi(a);
    }

    return output;
}

