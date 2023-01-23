#include "main.hpp"
#include "program/program.hpp"

#ifdef __linux__
#include <unistd.h>
#endif

void checkForRoot();

int main(int argc, char* argv[])
{
    // If on Linux, make sure program isn't running as root or sudo, since we
    // place files into (probably) either the /usr/local/bin/ or /home/ folder
    checkForRoot();

    Program::initProgram();

    Program::beginProgramLoop();

    Program::quitProgram();

    return 0;
}

void checkForRoot()
{
#ifdef __linux__

    auto user = getuid();
    auto perms = geteuid();

    // Running directly as root
    if(!user || !perms)
    {
	std::cerr << "Cannot run as root! Please run as regular user.\n";
        exit(1);
    }

#endif
    }
