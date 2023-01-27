#include "gui_controller.hpp"

using Logger::log, fmt::format;

// Constructor
GUI::GUIController::GUIController()
{
    focused_widget = -1;
    next_id = 0;
}

GUI::GUIController::~GUIController() = default;


// Sends a click to the widget under the cursor
void GUI::GUIController::sendClick()
{
    // If there are no widgets, stop
    if(widgets.size() == 0) { return; }

    SDL_Point mousepos;
    SDL_GetMouseState(&mousepos.x, &mousepos.y);

    // Iterate backwards so that items rendered on top have priority
    for(size_t i = widgets.size() - 1; i >= 0; i--)
    {
        auto& widget = widgets.at(i);
        SDL_Rect rect = widget->getRect();

        // Visibility and collision checking
        if(!widget->isVisible()) { continue; }
        if(!SDL_PointInRect(&mousepos, &rect)) { continue; }

        if(widget->canInteract())
        {
            widget->sendClick();
        }

        if(widget->canFocus())
        {
            focused_widget = widget->getID();
        }

        // If it hits anything, return
        return;
    }
}



// Sends an event to the currently focused widget
void GUI::GUIController::sendEvent(SDL_Event event)
{
    int index = getWidgetIndex(focused_widget);

    if(index != -1)
    {
        widgets.at(index)->sendEvent(event);
    } else {
        log(format("GUICTRL: Tried sending event to invalid widget! ID: {:d}",
            focused_widget),
            Logger::ERROR);
            focused_widget = -1;
    }
}


// Calls all visible widgets' draw() methods
void GUI::GUIController::drawWidgets()
{
    for(size_t i = 0; i < widgets.size(); i++)
    {
        if(widgets.at(i)->isVisible())
        {
            widgets.at(i)->draw();
        }
    }
}


// Removes the widget with the given ID number
void GUI::GUIController::removeWidget(long id)
{
    int index = getWidgetIndex(id);

    // If focused, remove focus state
    if(id == focused_widget) { focused_widget = -1; }

    if(index != -1)
    {
        widgets.erase(widgets.begin() + id);
    } else {
        log(format("GUICTRL: Tried removing invalid widget! ID: {:d}", id),
            Logger::ERROR);
    }
}



// Returns the index number of a widget with an ID. -1 for none found.
int GUI::GUIController::getWidgetIndex(long id)
{
    size_t left = 0;
    size_t right = widgets.size();

    while(left <= right)
    {
        int middle = left + (right - left) / 2;
        long widget_id = widgets.at(middle)->getID();

        if(widget_id == id) { return middle; }
        else if(widget_id < id) { left = middle + 1; }
        else { right = middle - 1; }
    }

    return -1;
}
