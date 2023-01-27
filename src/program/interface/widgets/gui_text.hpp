#pragma once

#include <SDL2/SDL.h>
#include "../../../core.hpp"
#include "../../window.hpp"
#include "../gui_widget.hpp"

namespace GUI
{
class Label : public Widget
{
public:
    Label();
    virtual ~Label();

    // Holds a set of Window:: functions to draw the widget
    void draw() const override;

    // Handles a mouse click
    void sendClick() override;

    // Handles an event
    void sendEvent(const SDL_Event& event) override;

    std::string getDisplay();
    void setDisplay(const std::string& new_display);

protected:
    std::string display;
};
}