#pragma once

#include "../../../core.hpp"
#include "../gui_menu.hpp"
#include "../gui_controller.hpp"
#include "../gui_widget.hpp"
#include "../widgets/gui_label.hpp"
#include "../widgets/gui_button.hpp"
#include "../../logger.hpp"

namespace GUI
{
class TestMenu : public Menu
{
public:
    TestMenu();
    virtual ~TestMenu();

    void initWidgets() override;

private:
};
}