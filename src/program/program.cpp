// Handles program initialization, loop, and cleanup

#include "program.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "interface/gui_controller.hpp"
#include "interface/gui_menu_controller.hpp"
#include "../utility/filedialogue.hpp"
#include <SDL_events.h>

using namespace Program;

using std::string, Logger::log;

ProgramStates programState = STOPPED;

GUI::GUIController gui;

constexpr double MAX_FRAMERATE = 59.7;
uint64_t frameStart, frameEnd;
double delta;


void Program::initProgram()
{
    Config::loadConfigFile();
    Logger::initLogger();
    Window::initWindow();
    log("PROGRAM: Fully initialized", Logger::logVERBOSE);
}



// NOTE: The program loop is run on the same thread function is called in
void Program::beginProgramLoop()
{
    GUI::MenuController::initMenus(gui);

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
                log("PROGRAM: Exit called.", Logger::logVERBOSE);
                break;
            } // End Quit
            case SDL_MOUSEBUTTONDOWN:
            {
                if(programState == MENU)
                {
                    gui.sendClick();
                    break;
                }
            } // End MouseButtonDown
            case SDL_KEYDOWN:
            {
                if(programState == MENU)
                {
                    gui.sendEvent(event);
                    break;
                }
            } // End Keydown
            }
        }

        Window::clearWindow();

        // Program handling
        switch(programState)
        {
        case MENU:
        {
            gui.drawWidgets();
            break;
        }
        case RUNNING:
        {
            break;
        }
        case STOPPED:
        {
            programState = MENU;
        }
        case EXITING: {}
        }

        Window::updateWindow();

        // Limit framerate
        frameEnd = SDL_GetPerformanceCounter();
        delta = (frameEnd - frameStart) / (double) SDL_GetPerformanceFrequency();
        if(delta < 1000/MAX_FRAMERATE)
        {
            SDL_Delay((1000 / MAX_FRAMERATE) - delta);
        }
    }

    log("PROGRAM: Exited main loop.", Logger::logVERBOSE);
}



// Exits all program subcomponents
void Program::quitProgram()
{
    Config::saveConfigFile();
    Logger::closeLogger();
    Window::closeWindow();
    GUI::MenuController::quitMenus();
}



// If the emulator is not running, prompts for a ROM file to start an emu,
// or just switches focus back to the existing emulator
void Program::startEmulator()
{
    // To be added when Emulator is added
    openFileDialogue("", {"gb", "gbc"});
}


// Closes the emulator if running and switches back to the menu.
void Program::quitEmulator()
{
    // To be added when Emulator is added
}


ProgramStates Program::getProgramState()
{
    return programState;
}



void Program::setProgramState(ProgramStates state)
{
    programState = state;
}