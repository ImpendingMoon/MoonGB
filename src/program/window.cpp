// Handles the window and rendering

#include "window.hpp"
#include "config.hpp"
#include "logger.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using std::string;
using Logger::log;
using fmt::format;

static constexpr int GB_X_RES = 160;
static constexpr int GB_Y_RES = 144;

SDL_Window* window;
SDL_Renderer* renderer;

std::string title;
std::array<SDL_Color, 5> color_palette;

void Window::initWindow()
{
    int err;

    title = "MoonGB";
    color_palette = Config::getColorPalette();

    window = SDL_CreateWindow(
             title.c_str(),
             SDL_WINDOWPOS_UNDEFINED,
             SDL_WINDOWPOS_UNDEFINED,
             Config::getLastWinX(),
             Config::getLastWinY(),
             0
             | SDL_WINDOW_SHOWN
             | SDL_WINDOW_RESIZABLE
             | SDL_WINDOW_ALLOW_HIGHDPI
             );

    if(window == NULL)
    {
        log(format("Could not create window in Window::initWindow! {:s}",
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
        log(format("Could not create renderer in Window::initWindow! {:s}",
                    SDL_GetError()), Logger::ERROR);
        exit(1);
    }

    // Have SDL do NN-scaling to handle resizing, entire program is GB-resolution
    err = SDL_RenderSetLogicalSize(renderer, GB_X_RES, GB_Y_RES);

    if(err != 0)
    {
        log(format("Could not set logical size in Window::initWindow! {:s}",
                    SDL_GetError()), Logger::ERROR);
        exit(1);
    }

    SDL_SetWindowMinimumSize(window, GB_X_RES, GB_Y_RES);

    // Initial render to screen, window will not show up without it
    SDL_Color color = color_palette[0];
    err = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if(err != 0)
    {
        log(format("Could not set bg draw color in Window::initWindow! {:s} ",
                   SDL_GetError()), Logger::ERROR);
        exit(1);
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}



void Window::closeWindow()
{
    SDL_DestroyWindow(window);
    // Causes Segfault. Does DestroyWindow also destroy attatched renderers?
    //SDL_DestroyRenderer(renderer);
}



void Window::updateWindow()
{
    SDL_RenderPresent(renderer);
}



void Window::clearWindow()
{
    SDL_Color color = color_palette[0];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}



void Window::refreshColorPalette()
{
    color_palette = Config::getColorPalette();
}
