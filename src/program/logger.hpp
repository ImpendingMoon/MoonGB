// Handles logging errors/debug messages to MoonGB.log, in either the root dir
// or inside of the Config file's PREF_PATH option.

#pragma once

#include <iostream>

namespace Logger
{

// Used in log() and the config file
// Will only log up to the level specified in config
enum LogLevel
{
    logNOTHING = 0, // Will not be logged/will log nothing
    logERROR = 1,   // Log program errors
    logVERBOSE = 2, // Log program information
    logDEBUG = 3,   // Log emulation errors
    logEXTREME = 4, // Log emulation info (SLOW AND BIG!) (like multiple GB log files)
};

// Use to initialize logger AFTER config has loaded
int initLogger();

// Closes the LogFile to properly clear buffers
void closeLogger();

// Puts a message in the console and/or log file.
void log(const std::string& message, LogLevel level);

};
