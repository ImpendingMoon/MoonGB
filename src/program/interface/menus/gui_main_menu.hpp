#pragma once

#include "../../../core.hpp"
#include "../gui_menu.hpp"

namespace GUI
{
class MainMenu : public Menu
{
public:
    MainMenu();
    ~MainMenu() override;
    // Stages widgets
    void initWidgets() override;

private:

};
}