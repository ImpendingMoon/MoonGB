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

    std::string getDisplay();
    void setDisplay(const std::string& new_display);

protected:
    std::string display;
};
}