// Handles the window and rendering

#pragma once

#include "../core.hpp"

namespace Window
{
    void initWindow();
    void closeWindow();

    void updateWindow();
    void clearWindow();

    // Pulls the color palette from the Config
    void refreshColorPalette();

};
