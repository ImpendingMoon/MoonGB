#include "gui_motd_menu.hpp"
#include "../widgets/gui_label.hpp"
#include <ctime>

using std::string, std::vector, fmt::format;
using std::shared_ptr, std::make_shared, std::move, GUI::Label;

const vector<string> messages = {
        "Fun Fact:\nThe Gameboy\nsold 118.69\nunits! Nice.",
        "Fun Fact:\nThe Gameboy \nruns at 59.7\nFPS!",
        "Fun Fact:\nThe Gameboy's\nCPU is called\nthe Sharp\nLR35902, and\nis based on the\n8080 and Z80.",
        "Fun Fact:\nThe Gameboy\nlaunched on\nApril 21 1989,\nand sold out on\nthe same day!",
        "Fun Fact:\nI can't be\nbothered to\nwrite more of\nthese right\nnow!",
        };

GUI::MOTD::MOTD() = default;
GUI::MOTD::~MOTD() = default;

void GUI::MOTD::initWidgets()
{
    string motd = messages.at(getMOTDHash(getDate()));

    shared_ptr<Label> label = make_shared<Label>();
    label->setDisplay(motd);
    label->setRect({58, 34, 0, 0});
    widgets.push_back(move(label));
}

string GUI::MOTD::getDate()
{
    auto now = time(0);
    auto ltime = localtime(&now);
    return format("{:04d}{:02d}{:02d}", 1900+ltime->tm_year, ltime->tm_mon + 1, ltime->tm_mday);
}


int GUI::MOTD::getMOTDHash(std::string date)
{
    int index = 0;

    for(char i : date)
    {
        index += (int)pow(i, 5);
        index -= (int)pow(index, 2);
    }

    index %= messages.size();
    return index;
}