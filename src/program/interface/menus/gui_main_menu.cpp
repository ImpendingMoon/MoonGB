#include "gui_main_menu.hpp"
#include "../widgets/gui_box.hpp"
#include "../widgets/gui_button.hpp"
#include "../widgets/gui_label.hpp"
#include "../../program.hpp"

using std::unique_ptr, std::make_unique, std::move, GUI::Button, GUI::Label;

GUI::MainMenu::MainMenu() = default;

GUI::MainMenu::~MainMenu() = default;

void GUI::MainMenu::initWidgets()
{
    // Labels
    unique_ptr<Label> label = make_unique<Label>();
    label->setDisplay("MoonGB");
    label->setRect({8, 8, 0, 0});
    widgets.push_back(move(label));

    // TODO: Place in separate menu
//    label = make_unique<Label>();
//    label->setDisplay("Now Playing:");
//    label->setRect({8, 121, 0, 0});
//    widgets.push_back(move(label));
//
//    label = make_unique<Label>();
//    label->setDisplay("|\\_(~)_/|");
//    label->setRect({8, 132, 0, 0});
//    widgets.push_back(move(label));

    // Buttons
    unique_ptr<Button> button = make_unique<Button>();
    button->setDisplay("Play");
    button->setRect({8, 33, 40, 12});
    button->setOnClick([](){ Program::startEmulator(); });
    widgets.push_back(move(button));

    button = make_unique<Button>();
    button->setDisplay("Stop");
    button->setRect({8, 55, 40, 12});
    button->setOnClick([](){ Program::quitEmulator(); });
    widgets.push_back(move(button));

    button = make_unique<Button>();
    button->setDisplay("Option");
    button->setRect({8, 77, 40, 12});
    widgets.push_back(move(button));

    button = make_unique<Button>();
    button->setDisplay("Quit");
    button->setRect({8, 99, 40, 12});
    button->setOnClick([](){ Program::setProgramState(Program::EXITING); });
    widgets.push_back(move(button));

    // Box
    unique_ptr<Box> box = make_unique<Box>();
    box->setRect({ 56, 32, 96, 80});
    widgets.push_back(move(box));
}
