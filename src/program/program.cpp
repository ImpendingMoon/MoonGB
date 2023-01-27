// Handles program initialization, loop, and cleanup

#include "program.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "interface/gui_controller.hpp"
#include "interface/widgets/gui_text.hpp"
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
    using std::unique_ptr, std::make_unique, GUI::Label;

    unique_ptr<Label> text = make_unique<Label>();
    text->setDisplay("Hello, World!");
    text->setRect({45, 1, 0, 0});
    gui.addWidget(text);

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