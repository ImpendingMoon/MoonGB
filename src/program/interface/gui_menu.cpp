// Container for several Widgets, and the logic to load/unload them

#include "gui_menu.hpp"


GUI::Menu::Menu() = default;

GUI::Menu::~Menu() = default;

void GUI::Menu::loadMenu(GUIController &gui)
{
    for(auto& widget : widgets)
    {
        long id = gui.addWidget(widget);
        widget_ids.push_back(id);
    }
    // Data inside of vector is no longer valid due to std::move
    widgets.erase(widgets.begin(), widgets.end());
}

void GUI::Menu::quitMenu(GUIController &gui)
{
    for(long id : widget_ids)
    {
        gui.removeWidget(id);
    }
    widget_ids.erase(widget_ids.begin(), widget_ids.end());
}

void GUI::Menu::initWidgets() {}