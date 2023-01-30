#pragma once

#include "../../../core.hpp"
#include "../gui_menu.hpp"

namespace GUI
{
class MOTD : public Menu
{
public:
    MOTD();
    ~MOTD();

    void initWidgets() override;

private:
    // Hashes the date into an index for the MOTD messages
    int getMOTDHash(std::string date);
    std::string getDate();
};
}