#include "gui_label.hpp"

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



std::string GUI::Label::getDisplay()
{
    return display;
}



void GUI::Label::setDisplay(const std::string& new_display)
{
    display = new_display;
}
