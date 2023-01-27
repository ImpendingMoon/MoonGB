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

    // Stages widgits
    virtual void initWidgets();
    // Loads staged widgits into a GUIController
    virtual void loadMenu(GUIController& gui);
    // Removes all the widgets with IDs in widget_ids from the GUIController
    virtual void quitMenu(GUIController& gui);

protected:
    // A list of widget id's that the menu has created
    std::vector<long> widget_ids;
    // Staged widgits ready to be added to a GUIContainer
    std::vector<std::unique_ptr<Widget>> widgets;
};
}