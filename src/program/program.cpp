// Handles program initialization, loop, and cleanup

#include "program.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "interface/gui_controller.hpp"
#include "interface/gui_menu_controller.hpp"
#include "../utility/filedialogue.hpp"
#include "../emulator/gameboy.hpp"
#include <SDL_events.h>

#define VERSION "0.3.0-dev"

using namespace Program;
using std::string, Logger::log, std::unique_ptr, std::make_unique;

ProgramStates programState = STOPPED;

GUI::GUIController gui;
unique_ptr<Gameboy> gb;

constexpr double MAX_FRAMERATE = 59.7;
uint64_t frameStart, frameEnd;
double delta;


void Program::initProgram()
{
    Config::loadConfigFile();
    Logger::initLogger();
    log("Starting MoonGB v" VERSION, Logger::logVERBOSE);
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

                if(programState == RUNNING)
                {
                    switch(event.key.keysym.sym)
                    {
                    // ESC switches back to menu
                    case SDLK_ESCAPE:
                    {
                        programState = MENU;
                    }
                    }
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
            if(!gb)
            {
                log("PROGRAM: Entered RUNNING without valid emulated system!",
                    Logger::logERROR);
                programState = MENU;
                break;
            }

            while(gb->getCycle() < gb->getCyclesPerFrame() && programState == RUNNING)
            {
                gb->step();
            }
            gb->resetCycle();
            log("PROGRAM: Finished frame.", Logger::logEXTREME);

            break;
        }
        case STOPPED:
        {
            if(gb) { quitEmulator(); }
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
    if(gb) { gb.reset(); }
    Config::saveConfigFile();
    Logger::closeLogger();
    Window::closeWindow();
    GUI::MenuController::quitMenus();
}



// If the emulator is not running, prompts for a ROM file to start an emu,
// or just switches focus back to the existing emulator
void Program::startEmulator()
{
    // If emulator already running, just switch to it
    if(gb)
    {
        programState = RUNNING;
        return;
    }

    bool emulator_started = false;

    while(!emulator_started)
    {
        // To be added when Emulator is added
        string path = openFileDialogue("", "Gameboy ROM", {"GB", "GBC"});

        if(!path.empty())
        {
            log("PROGRAM: Opened file " + path, Logger::logVERBOSE);
        } else {
            break; // If path is "" then user cancelled
        }

        try {
            gb = make_unique<Gameboy>(path);
        } catch(std::runtime_error& ex) {
            continue;
        } catch(std::invalid_argument& ex) {
            continue;
        }

        emulator_started = true;
        programState = RUNNING;
        GUI::MenuController::setNowPlaying(gb->getGameTitle());
    }
}


// Closes the emulator if running and switches back to the menu.
void Program::quitEmulator()
{
    programState = MENU;
    gb->dumpSystem();
    gb.reset();
    GUI::MenuController::setNowPlaying("|\\_(~)_/|");
    log("PROGRAM: Stopped emulator.", Logger::logVERBOSE);
}


ProgramStates Program::getProgramState()
{
    return programState;
}



void Program::setProgramState(ProgramStates state)
{
    programState = state;
}