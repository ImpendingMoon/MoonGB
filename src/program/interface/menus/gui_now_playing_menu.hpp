#pragma once
#include "../../../core.hpp"
#include "../gui_menu.hpp"
#include "../widgets/gui_label.hpp"

namespace GUI
{
class NowPlaying : public Menu
{
public:
    NowPlaying();
    ~NowPlaying();

    void initWidgets() override;

    void setDisplay(std::string display);

private:
};
}