#include "gui_button.hpp"

// Constructor/Destructor
GUI::Button::Button()
{
    display = "";
    on_click = nullptr;
    rect = {0,0,64,12};
    enabled = true;
    visible = true;
    can_interact = true;
    can_focus = false;
}

GUI::Button::~Button() = default;



void GUI::Button::draw() const
{
    Window::fillPRect(Window::TILE1, &rect);
    Window::drawPRect(Window::TILE2, &rect);
    Window::drawString(display, rect.x + 2, rect.y + 1);
}


void GUI::Button::sendClick()
{
    if(on_click != nullptr)
    {
        on_click();
    }
}


void GUI::Button::setOnClick(std::function<void()> function)
{
    on_click = function;
}


void GUI::Button::setDisplay(const std::string& new_display)
{
    display = new_display;
}