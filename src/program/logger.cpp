// Handles logging errors/debug messages to MoonGB.log, in either the root dir
// or inside of the Config file's PREF_PATH option.

#include "logger.hpp"

#include <filesystem>
#include <fstream>
#include <ctime>
#include "config.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::ofstream;
using fmt::format;

bool initialized = false;

using namespace Logger;

LogLevel log_level;

bool log_to_stdout;
bool log_to_logfile;

string log_file_path;
ofstream LogFile;

string getTimestamp();


// Use to initialize logger AFTER config has loaded
int Logger::initLogger()
{
	using std::filesystem::exists;
	using std::filesystem::create_directories;
    using std::filesystem::filesystem_error;

    log_file_path = Config::getOption("PrefPath");

    try {
        if(!exists(log_file_path)) { create_directories(log_file_path); }

    } catch(filesystem_error& ex) {
        cerr << "LOGGER: Cannot create log file directory!\n";
        cerr << "Log file path: " << log_file_path << "\n";
        cerr << ex.what() << "\n";

        return -1;
    }

    log_file_path.append("MoonGB.log");
    LogFile.open(log_file_path, std::ios_base::out);

    if(!LogFile.is_open())
    {
        cerr << "LOGGER: Cannot open log file!\n";
        return -1;
    }

	return 0;
}

// Closes the LogFile to properly clear buffers
void Logger::closeLogger()
{
    if(LogFile.is_open())
    {
        LogFile.close();
    }
}


// Puts a message in the console and/or log file.
void Logger::log(const std::string& message, LogLevel level)
{
	if(level <= log_level)
	{
		if(log_to_stdout)
		{
			cout << getTimestamp() << " " + message + "\n";
		}
		if(log_to_logfile)
		{
            LogFile << getTimestamp() << " " + message + "\n";
		}
	}
}


string getTimestamp()
{
	time_t now = std::time(0);
	tm* ltime = localtime(&now);

	return format("[{:02d}:{:02d}:{:02d}]",
		   ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
}
