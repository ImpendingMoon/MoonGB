// Handles the window and rendering

#include "window.hpp"

using std::string;

Window::Window()
{
    this->title = "MoonGB";
    this->bg_color = {0,0,0,0};

    window = SDL_CreateWindow(
             title.c_str(),
             SDL_WINDOWPOS_UNDEFINED,
             SDL_WINDOWPOS_UNDEFINED,
             MIN_X_SIZE,
             MIN_Y_SIZE,
             SDL_WINDOW_SHOWN
             );
}

Window::Window(const std::string& title, SDL_Color bg_color)
{
    this->title = title;
    this->bg_color = bg_color;
}
