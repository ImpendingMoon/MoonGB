#include "gui_widget.hpp"

// Constructor should be implemented by sub-class
GUI::Widget::Widget()
{
    rect = {0,0,0,0};
    enabled = false;
    visible = false;
    can_interact = false;
    can_focus = false;
    id = -1;
}

GUI::Widget::~Widget() = default;
void GUI::Widget::draw() const {}
void GUI::Widget::sendClick() {}
void GUI::Widget::sendEvent(const SDL_Event &event) {}

// Getters
SDL_Rect GUI::Widget::getRect() const { return rect; }

long GUI::Widget::getID() const { return id; }

bool GUI::Widget::isEnabled() const { return enabled; }

bool GUI::Widget::isVisible() const { return visible; }

bool GUI::Widget::canInteract() const { return can_interact; }

bool GUI::Widget::canFocus() const { return can_focus; }

// Setters
void GUI::Widget::setRect(const SDL_Rect& _rect) { rect = _rect; }

void GUI::Widget::setID(long value) { id = value; }

void GUI::Widget::setEnabled(bool value) { enabled = value; }

void GUI::Widget::setVisible(bool value) { }

void GUI::Widget::setCanInteract(bool value) { }

void GUI::Widget::setCanFocus(bool value) { }
