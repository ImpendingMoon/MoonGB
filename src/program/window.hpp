// Handles the window and rendering

#pragma once

#include <SDL2/SDL.h>
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
    // Wrapper for SDL_RenderWindowToLogical()
    void renderWindowToLogical(int windowX, int windowY,
                               float* logicalX, float* logicalY);

    // Drawing stuff //
    // Draws an array of PaletteIDs to the screen
    void drawPImage(const PaletteID* data, int x, int y, int width, int height);
    // Draws a point with a given palette color
    void drawPPoint(const PaletteID& color, int x, int y);
    // Draws a line with a given palette color
    void drawPLine(const PaletteID& color, int x1, int y1, int x2, int y2);
    // Draws a rect with a given palette color
    void drawPRect(const PaletteID& color, int x1, int y1, int width, int height);
    void drawPRect(const PaletteID& color, const SDL_Rect* rect);
    // Draws a filled rect with a given palette color
    void fillPRect(const PaletteID& color, int x1, int y1, int width, int height);
    void fillPRect(const PaletteID& color, const SDL_Rect* rect);

    // Draws a string to the screen using Tile3 of the color palette
    void drawString(const std::string& message, int x, int y);

    struct PImage
    {
        std::vector<PaletteID> data;
        int width;
        int height;
    };
};
