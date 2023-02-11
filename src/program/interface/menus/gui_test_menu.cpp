#include "gui_test_menu.hpp"

using std::shared_ptr, std::make_shared, std::move, GUI::Button, GUI::Label;

GUI::TestMenu::TestMenu()
{
    initWidgets();
}

GUI::TestMenu::~TestMenu() {}

void testButton1()
{
    log("Pressed Button 1", Logger::logVERBOSE);
}

void testButton2()
{
    log("Pressed Button 2", Logger::logVERBOSE);
}

void testButton3()
{
    log("Pressed Button 3", Logger::logVERBOSE);
}

void GUI::TestMenu::initWidgets() {
    shared_ptr<Label> label = make_shared<Label>();
    label->setDisplay("TEST MENU :D");
    label->setRect({36, 1, 0, 0});
    widgets.push_back(move(label));

    shared_ptr<Button> button1 = make_shared<Button>();
    button1->setDisplay("Option 1");
    button1->setRect({48, 60, 55, 12});
    button1->setOnClick(testButton1);
    widgets.push_back(move(button1));

    shared_ptr<Button> button2 = make_shared<Button>();
    button2->setDisplay("Option 2");
    button2->setRect({48, 92, 55, 12});
    button2->setOnClick(testButton2);
    widgets.push_back(move(button2));

    shared_ptr<Button> button3 = make_shared<Button>();
    button3->setDisplay("3");
    button3->setRect({56, 94, 12, 12});
    button3->setOnClick(testButton3);
    widgets.push_back(move(button3));
}