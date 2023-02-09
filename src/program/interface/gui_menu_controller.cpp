#include "gui_menu_controller.hpp"
#include "../program.hpp"
#include "menus/gui_main_menu.hpp"
#include "menus/gui_motd_menu.hpp"

using std::string, Logger::log;
using namespace GUI;

GUIController* guiController;

// Menu objects
MainMenu mainMenu;
MOTD motd;

// Initializes and loads the main menu in a given GUIController
void MenuController::initMenus(GUIController& gui)
{
    guiController = &gui;

    // Initialization
    mainMenu.initWidgets();
    motd.initWidgets();

    // Loading
    mainMenu.loadMenu(*guiController);
    motd.loadMenu(*guiController);
}



// Closes all currently loaded menus.
void MenuController::quitMenus()
{
    mainMenu.quitMenu(*guiController);
    motd.quitMenu(*guiController);
}


// Sets the game title in the Now Playing menu
void MenuController::setNowPlaying(const string& display)
{
    // To be implemented when NowPlaying is spun off into its own menu
}


// Logs an error, and shows a visual popup with the error
void MenuController::throwError(const string& display, Logger::LogLevel severity)
{
    // To be implemented when ErrorPopup is added
}
