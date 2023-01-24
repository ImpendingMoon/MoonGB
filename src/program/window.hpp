// Handles the window and rendering

#pragma once

#include "../core.hpp"

namespace Window
{
    enum PaletteID
    {
        BG=0,
        TILE0,
        TILE1,
        TILE2,
        TILE3,
    };

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

    // Drawing stuff //
    // Draws an array of PaletteIDs to the screen
    void drawPImage(PaletteID* data, int x, int y, int width, int height);

    // Draws a string to the screen using Tile3 of the color palette
    void drawString(const std::string& message, int x, int y);

};
