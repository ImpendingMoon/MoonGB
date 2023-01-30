#include "gui_box.hpp"
#include "../../window.hpp"

GUI::Box::Box()
{
    enabled = true;
    visible = true;
    can_interact = false;
    can_focus = false;
    rect = {0,0,8,8};
}

GUI::Box::~Box() = default;

void GUI::Box::draw() const
{
    Window::fillPRect(Window::TILE1, &rect);
    Window::drawPRect(Window::TILE2, &rect);
}