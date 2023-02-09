// Handles the creation and deletion of menus
#pragma once

#include "../../core.hpp"
#include "../logger.hpp"
#include "gui_controller.hpp"

namespace GUI::MenuController
{
    // Initializes and loads the main menu in a given GUIController
    void initMenus(GUIController& guiController);
    // Closes all currently loaded menus.
    void quitMenus();
    // Sets the game title in the Now Playing menu
    void setNowPlaying(const std::string& display);
    // Logs an error, and shows a visual popup with the error
    void throwError(const std::string& display, Logger::LogLevel severity);
}