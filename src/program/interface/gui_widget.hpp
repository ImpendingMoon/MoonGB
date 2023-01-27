#pragma once

#include <SDL2/SDL.h>
#include "../window.hpp"

namespace GUI
{
class Widget
{
public:
    Widget();
    virtual ~Widget();

    // Holds a set of Window:: functions to draw the widget
    virtual void draw() const;

    // Handles a mouse click
    virtual void sendClick();
    // Handles an event
    virtual void sendEvent(const SDL_Event& event);

    SDL_Rect getRect() const;
    long getID() const;
    bool isEnabled() const;
    bool isVisible() const;
    bool canInteract() const;
    bool canFocus() const;

    void setRect(const SDL_Rect& _rect);
    void setID(long value);
    void setEnabled(bool value);
    void setVisible(bool value);
    void setCanInteract(bool value);
    void setCanFocus(bool value);

protected:
    // Holds the position and size in one struct
    SDL_Rect rect;

    bool enabled; // The element can currently be interacted with
    bool visible; // The element should be drawn to the screen
    bool can_interact; // The element can be interacted with
    bool can_focus; // The element can take keyboard focus

    long id; // Assigned by the GUIController
};
}