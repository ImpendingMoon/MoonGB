// Handles the window and rendering

#pragma once

#include "../core.hpp"

namespace Window
{
    void initWindow();
    void closeWindow();


    // Wrapper for SDL_RenderPresent()
    void updateWindow();
    // Draws the background color to the screen
    void clearWindow();

    // Pulls the color palette from the Config, will display next clear+update
    void refreshColorPalette();

    // Wrapper for SDL_GetWindowSize()
    void getWindowSize(int* width, int* height);

};
