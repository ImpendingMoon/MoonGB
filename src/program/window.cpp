// Handles the window and rendering

#include "window.hpp"
#include "config.hpp"
#include "logger.hpp"
#include <SDL2/SDL.h>

using std::string, Logger::log, Config::getOption, fmt::format;

static constexpr int GB_X_RES = 160;
static constexpr int GB_Y_RES = 144;

SDL_Window* window;
SDL_Renderer* renderer;

std::string title;
std::array<SDL_Color, 5> color_palette;

void Window::initWindow()
{
    using std::stoi;

    int err;

    title = "MoonGB";
    refreshColorPalette();

    window = SDL_CreateWindow(
             title.c_str(),
             SDL_WINDOWPOS_UNDEFINED,
             SDL_WINDOWPOS_UNDEFINED,
             stoi(getOption("WinSizeX")),
             stoi(getOption("WinSizeY")),
             0
             | SDL_WINDOW_SHOWN
             | SDL_WINDOW_RESIZABLE
             | SDL_WINDOW_ALLOW_HIGHDPI
             );

    if(window == NULL)
    {
        log(format("WINDOW: Could not create window! {:s}",
                    SDL_GetError()), Logger::ERROR);
        exit(1);
    }

    renderer = SDL_CreateRenderer(
               window,
               -1,
               0
               );

    if(renderer == NULL)
    {
        log(format("WINDOW: Could not create renderer! {:s}",
                    SDL_GetError()), Logger::ERROR);
        exit(1);
    }

    // Have SDL use nearest-neighbor scaling to handle resizing.
    err = SDL_RenderSetLogicalSize(renderer, GB_X_RES, GB_Y_RES);

    if(err != 0)
    {
        log(format("WINDOW: Could not set logical size! {:s}",
                    SDL_GetError()), Logger::ERROR);
        exit(1);
    }

    SDL_SetWindowMinimumSize(window, GB_X_RES, GB_Y_RES);

    // Initial render to screen, window will not show up without it
    clearWindow();
    updateWindow();

    log("WINDOW: Window successfully created.", Logger::VERBOSE);
}


void Window::closeWindow()
{
    SDL_DestroyWindow(window);
    // Causes Segfault. Does DestroyWindow also destroy attatched renderers?
    //SDL_DestroyRenderer(renderer);
}



// Wrapper for SDL_RenderPresent()
void Window::updateWindow()
{
    SDL_RenderPresent(renderer);
}



// Wrapper for SDL_RenderPresent()
void Window::clearWindow()
{
    // Black letterboxing
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    // BG-colored usable screen
    SDL_Color color = color_palette[0];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, NULL);
}



// Pulls the color palette from the Config, will display next clear+update
void Window::refreshColorPalette()
{
    using Config::stringToPalette, Config::getOption;
    color_palette = stringToPalette(getOption("ColorPalette"));

    log("WINDOW: Loaded color palette from config.", Logger::VERBOSE);
}
