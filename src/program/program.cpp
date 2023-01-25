// Handles program initialization, loop, and cleanup

#include "program.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include <SDL_events.h>

using namespace Program;

using std::string, Logger::log;

ProgramStates programState = STOPPED;

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

        // Rendering
        Window::clearWindow();
        Window::drawString("!\"#$%&\'()*+,-./0", 0, 0);
        Window::drawString("123456789:;<=>?@", 0, 8+1);
        Window::drawString("ABCDEFGHIJKLMNOP", 0, 16+2);
        Window::drawString("QRTSUVWXYZ[\\]^_`", 0, 24+3);
        Window::drawString("abcdefghijklmnop", 0, 32+4);
        Window::drawString("qrstuvwxyz{|}~", 0, 40+5);

        Window::drawString("Hello, World!", 0, 61);
        Window::drawString("Testing text rendering...", 0, 69+1);
        Window::drawString("C(n, r) = n!/(r!(n-r)!)", 0, 77+2);
        Window::drawString("'q' has a stray pixel. Why?", 0, 85+3);
        Window::drawString("The Gameboy can produce", 0, 104);
        Window::drawString("mind-boggling effects!", 0, 112+1);

        drawShrug();

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



int shrugx = 160/2-20, shrugy = 0, speedx = 1, speedy = 1;
void drawShrug()
{
    string shrug{};
    shrug.append("|\\_(");
    shrug.push_back(126);
    shrug.append(")_/|");

    shrugx += speedx;
//    shrugy += speedy;
//
    if(shrugx <= 0 || shrugx >= (160 - 64)) { speedx *= -1; }
//    if(shrugy <= 0 || shrugy >= (144 - 8)) { speedy *= -1; }

    Window::drawString(shrug, shrugx, 144-8-6);
}