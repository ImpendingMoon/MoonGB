// Handles program initialization, loop, and cleanup

#include "program.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "interface/gui_controller.hpp"
#include "interface/menus/gui_main_menu.hpp"
#include "interface/menus/gui_motd_menu.hpp"
#include <SDL_events.h>

using namespace Program;

using std::string, Logger::log;

ProgramStates programState = STOPPED;

GUI::GUIController gui;

constexpr double MAX_FRAMERATE = 59.7;
uint64_t frameStart, frameEnd;
double delta;

// The most important function (mostly for test purposes);
void drawShrug();


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
    std::unique_ptr<GUI::MainMenu> menu = std::make_unique<GUI::MainMenu>();
    menu->initWidgets();
    menu->loadMenu(gui);
    std::unique_ptr<GUI::MOTD> motd = std::make_unique<GUI::MOTD>();
    motd->initWidgets();
    motd->loadMenu(gui);

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

    log("PROGRAM: Exited main loop.", Logger::VERBOSE);
}

// Exits all program subcomponents
void Program::quitProgram()
{
    Config::saveConfigFile();
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