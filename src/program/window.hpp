// Handles the window and rendering

#pragma once

#include "../core.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Window
{
public:
    Window();
    Window(const std::string& title, SDL_Color bg_color);

private:
    // The minimum window dimensions, set to GB resolution
    static constexpr uint32_t MIN_X_SIZE = 160;
    static constexpr uint32_t MIN_Y_SIZE = 144;

    SDL_Window* window;
    SDL_Renderer* renderer;

    std::string title;
    SDL_Color bg_color;
};
