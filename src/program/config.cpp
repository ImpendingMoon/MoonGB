// Handles the config file and pref paths

#include "config.hpp"

#define COMPANY "ImpendingMoon"
#define PROGRAM "MoonGB"

#include <fstream>
#include <algorithm>
#include <cstdio>
#include <string>
#include "window.hpp"
#include "logger.hpp"

using std::string, std::map, std::array, std::ifstream, std::ofstream;
using fmt::format, Logger::log;

map<string, string> options{};

// Loads a properly formatted .ini file into options
void parseConfigFile(ifstream& ConfFile);
// Ensures that every key in DEF_OPTIONS also exists inside of options
void validateOptions();

// Defaults
const map<string, string> DEF_OPTIONS {
    {"PrefPath", "./"},
    {"LogLevel", "3"},
    {"LogToStdout", "1"},
    {"LogToLogFile", "1"},
    {"WinSizeX", "640"},
    {"WinSizeY", "576"},
    // Default color palette based off of Gameboy Pocket
    {"ColorPalette", "{196,207,161,000} " // BG
                     "{196,207,161,000} " // Tile0
                     "{139,149,109,000} " // Tile1
                     "{077,083,060,000} " // Tile2
                     "{031,031,031,000}"  // Tile3
    },
};



// Attempts to find and load config options from the file
// Loads defaults if file/option not found
void Config::loadConfigFile()
{
    // Load from configuration file
    ifstream ConfFile;

    // Try finding MoonGB.ini in current directory
    ConfFile.open("./MoonGB.ini", std::ios_base::in);
    // If failed, try finding in SDL_GetPrefPath
    if(!ConfFile.is_open())
    {
        ConfFile.open( format("{:s}MoonGB.ini",
                       SDL_GetPrefPath(COMPANY, PROGRAM)),
                       std::ios_base::in);
    }
    // If failed, abort and load defaults
    if(!ConfFile.is_open())
    {
        resetAllOptions();
    }

    // Load .ini into options
    parseConfigFile(ConfFile);

    ConfFile.close();

    // Check if directory specified in options is writable for log
    string testFilePath = format("{:s}temp", options.at("PrefPath"));
    ofstream testFile;
    // Try writing a temp file to the directory, then delete it
    testFile.open(testFilePath);

    if(!testFile.is_open()) {
        std::cout << format("Cannot write to path {:s}! Trying user directory...\n",
                            options.at("PrefPath"));

        options["PrefPath"] = SDL_GetPrefPath(COMPANY, PROGRAM);
        testFilePath = format("{:s}/temp", options.at("PrefPath"));
        testFile.open(testFilePath);
    }

    if(!testFile.is_open())
    {
        std::cerr << format("Cannot write to path {:s}! Aborting...\n",
                            options.at("PrefPath"));

    } else {
        testFile.close();
        std::remove(testFilePath.c_str());
    }

}

int Config::saveConfigFile()
{
    using std::to_string;
    // Fetch updates from places that don't automatically update
    int w, h;
    Window::getWindowSize(&w, &h);
    options["WinSizeX"] = to_string(w);
    options["WinSizeY"] = to_string(h);

    ofstream ConfFile;

    // Try opening MoonGB.ini in current directory
    ConfFile.open("./MoonGB.ini", std::ios_base::out);
    // If failed, try finding in SDL_GetPrefPath
    if(!ConfFile.is_open())
    {
        ConfFile.open( format("{:s}/MoonGB.ini",
                       SDL_GetPrefPath(COMPANY, PROGRAM)),
                       std::ios_base::out );
    }
    // If failed, abort.
    if(!ConfFile.is_open())
    {
        log(format("CONFIG: Could not open {:s}MoonGB.ini to save! Aborting...",
            SDL_GetPrefPath(COMPANY, PROGRAM)),
            Logger::ERROR);
        return -1;
    }

    // Save all key value pairs to .ini file
    for(const auto& [key, value] : options)
    {
        ConfFile << key << "=" << value << "\n";
    }

    log("CONFIG: Successfully saved MoonGB.ini.", Logger::VERBOSE);

    ConfFile.close();

    return 0;
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

    // Trim last whitespace
    output = output.substr(0, output.length() - 1);

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
        options["ColorPalette"] = DEF_OPTIONS.at("ColorPalette");
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
        int offset = (int)i * 18;

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



// Loads a properly formatted .ini file into options
void parseConfigFile(ifstream& ConfFile)
{
    using std::getline;

    // Read lines until EOF is reached
    string line{};
    while( getline(ConfFile, line) )
    {
        // If empty or starting with a comment, skip.
        if(line.empty()) { continue; }
        if(line.starts_with(";") || line.starts_with("#")) { continue; }

        // If line has comment, remove section with comment
        auto index = line.find_first_of(";#");
        if(index != std::string::npos)
        {
            line = line.substr(0, index);
        }

        // Find '=' for key,pair
        index = line.find('=');
        if(index == std::string::npos) { continue; }

        string value = line.substr(index + 1);
        string key = line.substr(0, index);


        // Trim whitespace from key and value
        size_t keyStartIndex = key.find_first_not_of(" \t");
        size_t keyEndIndex = key.find_last_not_of(" \t");
        size_t valueStartIndex = value.find_first_not_of(" \t");
        size_t valueEndIndex = value.find_last_not_of(" \t");

        if(keyStartIndex != 0 && keyEndIndex != key.length())
        {
            key = key.substr(keyStartIndex, keyEndIndex);
        }
        if(valueStartIndex != 0 && valueEndIndex != value.length())
        {
            value = value.substr(valueStartIndex, valueEndIndex);
        }


        // Only pass into options if a default option exists
        // Prevents arbitrary data from being loaded into options
        if(DEF_OPTIONS.count(key))
        {
            options[key] = value;
        }
    }

    // Ensure that all options, even if not in .ini file, are loaded
    validateOptions();
}

// Ensures that every key in DEF_OPTIONS also exists inside of options
void validateOptions()
{
    for(const auto& [key, value] : DEF_OPTIONS)
    {
        if(!options.count(key)) { options[key] = value; }
    }
}