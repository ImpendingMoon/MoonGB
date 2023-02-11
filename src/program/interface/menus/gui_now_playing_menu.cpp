#include "gui_now_playing_menu.hpp"

using std::string, std::shared_ptr, std::make_shared;

GUI::NowPlaying::NowPlaying() = default;
GUI::NowPlaying::~NowPlaying() = default;


void GUI::NowPlaying::initWidgets()
{
    shared_ptr<Label> label = make_shared<Label>();
    // Now Playing
    label = make_shared<Label>();
    label->setDisplay("Now Playing:");
    label->setRect({8, 121, 0, 0});
    widgets.push_back(std::move(label));

    // Display
    label = make_shared<Label>();
    label->setDisplay("|\\_(~)_/|");
    label->setRect({8, 132, 0, 0});
    widgets.push_back(std::move(label));
}


void GUI::NowPlaying::setDisplay(std::string display)
{
    long id = widget_ids[1];
    // Pull widget pointer from GUI framework
    // remind me to not make a gui framework again, this is terrible.
    auto label = std::dynamic_pointer_cast<Label>
            (guiController->getWidget<shared_ptr<Widget>>(id));

    if(label)
    {
        label->setDisplay(display);
    }
}
