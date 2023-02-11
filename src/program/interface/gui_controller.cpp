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
    if(widgets.empty()) { return; }

    SDL_Point mousepos;
    SDL_GetMouseState(&mousepos.x, &mousepos.y);
    float mouseposfx, mouseposfy;
    Window::renderWindowToLogical(mousepos.x, mousepos.y, &mouseposfx, &mouseposfy);
    mousepos.x = (int) mouseposfx;
    mousepos.y = (int) mouseposfy;

    // Iterate backwards so that items rendered on top have priority
    for(int i = widgets.size() - 1; i >= 0; i--)
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
        widgets.erase(widgets.begin() + index);
    } else {
        log(format("GUICTRL: Tried removing invalid widget! ID: {:d}", id),
            Logger::logERROR);
    }
}



// Returns the index number of a widget with an ID. -1 for none found.
int GUI::GUIController::getWidgetIndex(long id)
{
    for(size_t i = 0; i < widgets.size(); i++)
    {
        long widget_id = widgets.at(i)->getID();
        if(widget_id == id) { return i; }
    }

    return -1;
}
