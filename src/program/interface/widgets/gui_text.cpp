#include "gui_text.hpp"

// Constructor
GUI::Label::Label()
{
    enabled = false;
    visible = true;
    can_interact = false;
    can_focus = false;
}

// Destructor
GUI::Label::~Label() = default;


// Holds a set of Window:: functions to draw the widget
void GUI::Label::draw() const
{
    Window::drawString(display, rect.x, rect.y);
}


// Handles a mouse click
void GUI::Label::sendClick() {}


// Handles an event
void GUI::Label::sendEvent(const SDL_Event &event) {}



std::string GUI::Label::getDisplay()
{
    return display;
}



void GUI::Label::setDisplay(const std::string& new_display)
{
    display = new_display;
}
