#pragma once

#include <functional>
#include "../../../core.hpp"
#include "../gui_widget.hpp"

namespace GUI
{
class Button : public Widget
{
public:
    Button();
    virtual ~Button();

    void draw() const override;
    void sendClick() override;

    // Set the function that is called with sendClick()
    void setOnClick(std::function<void()> function);
    void setDisplay(const std::string& new_display);
    
protected:
    std::string display;
    std::function<void()> on_click;
};
}
