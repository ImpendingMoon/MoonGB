// Handles program initialization, loop, and cleanup

#include "program.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include <SDL_events.h>

using namespace Program;

using std::string;
using Logger::log;

ProgramStates programState = STOPPED;

constexpr double MAX_FRAMERATE = 59.7;
uint64_t frameStart, frameEnd;
double delta;

void Program::initProgram()
{
    Config::loadConfigFile();
    Logger::initLogger();
    Window::initWindow();
    log("PROGRAM: Fully initialized", Logger::VERBOSE);
}



// NOTE: The program loop is run on the same thread function is called in
void Program::beginProgramLoop()
{
    programState = MENU;

    while(programState != EXITING)
    {
        frameStart = SDL_GetPerformanceCounter();

        // Input processing
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
            {
                programState = EXITING;
                log("PROGRAM: Exit called.", Logger::VERBOSE);
                break;
            }
            }
        }

        // Program handling
        switch(programState)
        {
        case MENU:
        {
        }
        case RUNNING:
        {
        }
        case STOPPED:
        {
            programState = MENU;
        }
        case EXITING: {}
        }

        // Rendering
        Window::clearWindow();
        Window::updateWindow();

        frameEnd = SDL_GetPerformanceCounter();
        delta = (frameEnd - frameStart) / (double) SDL_GetPerformanceFrequency();
    }

    log("PROGRAM: Exited main loop.", Logger::VERBOSE);
}

// Exits all program subcomponents
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
