// Container for several Widgets, and the logic to load/unload them

#pragma once

#include "../../core.hpp"
#include "gui_widget.hpp"
#include "gui_controller.hpp"

namespace GUI
{
class Menu
{
public:
    Menu();
    virtual ~Menu();

    // Stages widgets
    virtual void initWidgets();
    // Loads staged widgets into a GUIController
    void loadMenu(GUIController& gui);
    // Removes all the widgets with IDs in widget_ids from the GUIController
    void quitMenu(GUIController& gui);
    // Used for accessing widgets from the GUIController after they've been loaded
    long getWidgetID(int index);

protected:
    // A list of widget id's that the menu has created
    std::vector<long> widget_ids;
    // Staged widgets ready to be added to a GUIContainer
    std::vector<std::shared_ptr<Widget>> widgets;
    // Pointer to the loaded GUIController
    GUIController* guiController;
};
}