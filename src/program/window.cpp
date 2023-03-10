// Handles the window and rendering

#include "window.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "./graphics/font.hpp"

using std::string, std::array, std::vector;
using Logger::log, Config::getOption, fmt::format;

static constexpr int GB_X_RES = 160;
static constexpr int GB_Y_RES = 144;

SDL_Window* window;
SDL_Renderer* renderer;

string title;
array<SDL_Color, 5> color_palette;

SDL_Texture* charMap = nullptr;

void Window::initWindow()
{
    using std::stoi;
    int err;

    // Load data
    title = "MoonGB";

    // FontData defined in fontSurface.hpp
    SDL_Surface* fontSurface;
    fontSurface = SDL_CreateRGBSurfaceWithFormatFrom(
                  (void*)&FontData.pixel_data,
                  (int)FontData.width,
                  (int)FontData.height,
                  8,
                  (int)(FontData.width * FontData.bytes_per_pixel),
                  SDL_PIXELFORMAT_RGBA8888
                  );

    refreshColorPalette();

    int winWidth, winHeight;
    try {
        winWidth = stoi(getOption("WinSizeX"));
        winHeight = stoi(getOption("WinSizeY"));

    } catch(std::invalid_argument& ex) {
        log("WINDOW: Couldn't load window size! Loading defaults...", Logger::logERROR);
        Config::resetOption("WinSizeX");
        Config::resetOption("WinSizeY");
        winWidth = stoi(getOption("WinSizeX"));
        winHeight = stoi(getOption("WinSizeY"));

    }


    // Create window/renderer
    window = SDL_CreateWindow(
             title.c_str(),
             SDL_WINDOWPOS_UNDEFINED,
             SDL_WINDOWPOS_UNDEFINED,
             winWidth,
             winHeight,
             0
             | SDL_WINDOW_SHOWN
             | SDL_WINDOW_RESIZABLE
             | SDL_WINDOW_ALLOW_HIGHDPI
             );

    if(window == nullptr)
    {
        log(format("WINDOW: Could not create window! {:s}",
                    SDL_GetError()), Logger::logERROR);
        exit(1);
    }

    renderer = SDL_CreateRenderer(
               window,
               -1,
               0
               | SDL_RENDERER_PRESENTVSYNC
               );

    if(renderer == nullptr)
    {
        log(format("WINDOW: Could not create renderer! {:s}",
                    SDL_GetError()), Logger::logERROR);
        exit(1);
    }


    // Have SDL use nearest-neighbor scaling to handle resizing.
    err = SDL_RenderSetLogicalSize(renderer, GB_X_RES, GB_Y_RES);

    if(err != 0)
    {
        log(format("WINDOW: Could not set logical size! {:s}",
                    SDL_GetError()), Logger::logERROR);
        exit(1);
    }

    SDL_SetWindowMinimumSize(window, GB_X_RES, GB_Y_RES);


    // Load font into texture for SDL_RenderCopy
    charMap = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_Color color = color_palette[4];
    SDL_SetTextureColorMod(charMap, color.r, color.g, color.b);

    // Initial render to screen, window will not show up without it
    clearWindow();
    updateWindow();

    log("WINDOW: Successfully created window.", Logger::logVERBOSE);
}


void Window::closeWindow()
{
    SDL_DestroyTexture(charMap);
    SDL_DestroyWindow(window);
    // Causes Segfault. Does DestroyWindow also destroy attached renderers?
    //SDL_DestroyRenderer(renderer);
}



// Wrapper for SDL_RenderPresent()
void Window::updateWindow()
{
    SDL_RenderPresent(renderer);
}



// Clears the window and draws the background color
void Window::clearWindow()
{
    // Black letterboxing
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    // BG-colored usable screen
    SDL_Color color = color_palette[BG];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, nullptr);
}



// Pulls the color palette from the Config, will display next clear+update
void Window::refreshColorPalette()
{
    using Config::stringToPalette, Config::getOption;
    color_palette = stringToPalette(getOption("ColorPalette"));

    // Label color is always darkest, for simplicity
    SDL_Color color = color_palette[TILE3];
    SDL_SetTextureColorMod(charMap, color.r, color.g, color.b);

    log("WINDOW: Loaded color palette from config.", Logger::logVERBOSE);
}



// Wrapper for SDL_GetWindowSize()
void Window::getWindowSize(int* width, int* height)
{
    SDL_GetWindowSize(window, (int*)width, (int*)height);
}



// Wrapper for SDL_RenderWindowToLogical()
void Window::renderWindowToLogical(int windowX, int windowY,
                           float* logicalX, float* logicalY)
{
    SDL_RenderWindowToLogical(renderer, windowX, windowY, logicalX, logicalY);
}



// Drawing stuff //

// Draws an array of PaletteIDs to the screen
void Window::drawPImage(const PaletteID* data, int x, int y, int width, int height)
{
    // NOTE: This can and should be optimized. Probably by using a surface.

    // Tiles are stored into vectors of points to call SDL_RenderDrawPoints()
    // a few times instead of SDL_RenderDrawPoint() 23,040 times.
    array<vector<SDL_Point>, 5> points{};

    for (int i = 0; i < width * height; i++) {
        SDL_Point position{(x + (i % width)), (y + (i/width))};
        int TileID = data[i];
        // Push location into correct vector of tiles
        points[TileID].push_back(position);
    }

    // Skip index 0 because no need to draw BG pixels, since BG is BG-colored
    for(size_t i = 1; i < points.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer,
                               color_palette[i].r,
                               color_palette[i].g,
                               color_palette[i].b,
                               color_palette[i].a
        );
        SDL_RenderDrawPoints(renderer, points[i].data(), (int)points[i].size());
    }
}



// Draws a point with a given palette color
void Window::drawPPoint(const PaletteID& color, int x, int y)
{
    SDL_SetRenderDrawColor(renderer,
                           color_palette[color].r,
                           color_palette[color].g,
                           color_palette[color].b,
                           color_palette[color].a
    );
    SDL_RenderDrawPoint(renderer, x, y);
}


// Draws a line with a given palette color
void Window::drawPLine(const PaletteID& color, int x1, int y1, int x2, int y2)
{
    SDL_SetRenderDrawColor(renderer,
                           color_palette[color].r,
                           color_palette[color].g,
                           color_palette[color].b,
                           color_palette[color].a
    );
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}


// Draws a rect with a given palette color
void Window::drawPRect(const PaletteID& color, int x1, int y1, int width, int height)
{
    SDL_SetRenderDrawColor(renderer,
                           color_palette[color].r,
                           color_palette[color].g,
                           color_palette[color].b,
                           color_palette[color].a
    );
    SDL_Rect rect = {x1, y1, width, height};
    SDL_RenderDrawRect(renderer, &rect);
}


void Window::drawPRect(const PaletteID& color, const SDL_Rect* rect)
{
    SDL_SetRenderDrawColor(renderer,
                           color_palette[color].r,
                           color_palette[color].g,
                           color_palette[color].b,
                           color_palette[color].a
    );
    SDL_RenderDrawRect(renderer, rect);
}


// Draws a filled rect with a given palette color
void Window::fillPRect(const PaletteID& color, int x1, int y1, int width, int height)
{
    SDL_SetRenderDrawColor(renderer,
                           color_palette[color].r,
                           color_palette[color].g,
                           color_palette[color].b,
                           color_palette[color].a
    );
    SDL_Rect rect = {x1, y1, width, height};
    SDL_RenderFillRect(renderer, &rect);
}


void Window::fillPRect(const PaletteID& color, const SDL_Rect* rect)
{
    SDL_SetRenderDrawColor(renderer,
                           color_palette[color].r,
                           color_palette[color].g,
                           color_palette[color].b,
                           color_palette[color].a
    );
    SDL_RenderFillRect(renderer, rect);
}



// Draws a string to the screen using Tile3 of the color palette
void Window::drawString(const std::string& message, int x, int y)
{
    SDL_Rect sourceChar {0, 0, 8, 8 }; // Rect to copy to screen
    SDL_Rect destChar = {0, 0, 8, 8};
    int offset = 0; // Offset for next character
    int line = 0;

    for(char i : message)
    {
        // Check for newline
        if(i == '\n')
        {
            line++;
            offset = -4;
        }

        // Character Map starts with '!' at position 0
        int charIndex = i - '!';
        // Get X and Y position from index
        sourceChar.x = (charIndex % 16) * 8;
        sourceChar.y = (charIndex / 16) * 8;

        destChar.x = x + offset;
        // If a valid character, pull y-offset from FontInfo, otherwise 0.
        destChar.y = (charIndex > 0) ? line * 10 + y + FontInfo.y_offset[charIndex] : y + line * 10;

        SDL_RenderCopy(renderer,
                       charMap,
                       &sourceChar,
                       &destChar
        );

        // If a valid character, pull width from FontInfo, otherwise increment by 4.
        offset += (charIndex > 0) ? FontInfo.width[charIndex] + 1 : 4;
    }
}


