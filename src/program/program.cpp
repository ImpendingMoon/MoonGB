// Handles program initialization, loop, and cleanup

#include "program.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"

using namespace Program;

ProgramStates programState = STOPPED;


void Program::initProgram()
{
    Config::loadConfigFile();
    Logger::initLogger();
    Window::initWindow();
}

void Program::beginProgramLoop()
{
    // TODO: Program Loop
}

void Program::quitProgram()
{
    Config::closeConfigFile();
    Logger::closeLogger();
    Window::closeWindow();
}

ProgramStates Program::getProgramState()
{
    return programState;
}

void Program::setProgramState(ProgramStates state)
{
    programState = state;
}
