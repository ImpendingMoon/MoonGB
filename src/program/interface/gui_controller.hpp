#pragma once
#include <SDL2/SDL.h>
#include "../../core.hpp"
#include "gui_widget.hpp"
#include "../logger.hpp"

namespace GUI
{
class GUIController
{
public:
    GUIController();
    virtual ~GUIController();

    // Sends a click to the widget under the cursor
    void sendClick();
    // Sends an event to the currently focused widget
    void sendEvent(SDL_Event event);
    // Calls all visible widgets' draw() methods
    void drawWidgets();
    // Adds a widget, returns the ID number of the widget
    template<typename W>
    long addWidget(W& widget);
    // Removes the widget with the given ID number
    void removeWidget(long id);
    // Accesses the widget with the given ID number
    template<typename W>
    W getWidget(long id);
private:
    std::vector<std::shared_ptr<Widget>> widgets;
    long focused_widget; // ID for the focused widget. -1 for no focus.
    long next_id; // The ID that will be assigned to the next widget

    // Returns the index number of a widget with an ID. -1 for none found.
    int getWidgetIndex(long id);
};
}

template<typename W>
long GUI::GUIController::addWidget(W& widget)
{
    widget->setID(next_id);
    widgets.push_back(std::move(widget));
    return next_id++;
}

template<typename W>
W GUI::GUIController::getWidget(long id)
{
    int index = getWidgetIndex(id);
    if(index == -1) { return nullptr; }

    return widgets.at(index);
}