#pragma once

#include "../../../core.hpp"
#include "../gui_widget.hpp"

namespace GUI
{
class Box : public Widget
{
public:
    Box();
    virtual ~Box();

    void draw() const override;

private:
};
}