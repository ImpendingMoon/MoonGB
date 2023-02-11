#include "gui_main_menu.hpp"
#include "../widgets/gui_box.hpp"
#include "../widgets/gui_button.hpp"
#include "../widgets/gui_label.hpp"
#include "../../program.hpp"

using std::shared_ptr, std::make_shared, GUI::Button, GUI::Label;

GUI::MainMenu::MainMenu() = default;

GUI::MainMenu::~MainMenu() = default;

void GUI::MainMenu::initWidgets()
{
    // Labels
    shared_ptr<Label> label = make_shared<Label>();
    label->setDisplay("MoonGB");
    label->setRect({8, 8, 0, 0});
    widgets.push_back(std::move(label));

    // Buttons
    shared_ptr<Button> button = make_shared<Button>();
    button->setDisplay("Play");
    button->setRect({8, 33, 40, 12});
    button->setOnClick([](){ Program::startEmulator(); });
    widgets.push_back(std::move(button));

    button = make_shared<Button>();
    button->setDisplay("Stop");
    button->setRect({8, 55, 40, 12});
    button->setOnClick([](){ Program::quitEmulator(); });
    widgets.push_back(std::move(button));

    button = make_shared<Button>();
    button->setDisplay("Option");
    button->setRect({8, 77, 40, 12});
    widgets.push_back(std::move(button));

    button = make_shared<Button>();
    button->setDisplay("Quit");
    button->setRect({8, 99, 40, 12});
    button->setOnClick([](){ Program::setProgramState(Program::EXITING); });
    widgets.push_back(std::move(button));

    // Box
    shared_ptr<Box> box = make_shared<Box>();
    box->setRect({ 56, 32, 96, 80});
    widgets.push_back(std::move(box));
}
