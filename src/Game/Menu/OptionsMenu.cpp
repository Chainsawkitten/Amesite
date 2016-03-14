#include "OptionsMenu.hpp"

#include "MenuOption.hpp"
#include "MenuSwitch.hpp"
#include <MainWindow.hpp>
#include "Menu.hpp"

#include "../Util/GameSettings.hpp"

OptionsMenu::OptionsMenu(Menu* parentMenu) : SubMenu() {
    // Define menu options.
    std::vector<std::string> options;
    options.push_back("On");
    options.push_back("Off");
    MenuSwitch* menuSwitch = new MenuSwitch(GetFont(), "Fullscreen", options, glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::Fullscreen, this, std::placeholders::_1);
    AddMenuOption(menuSwitch);
    
    menuSwitch = new MenuSwitch(GetFont(), "FXAA", options, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::FXAA, this, std::placeholders::_1);
    AddMenuOption(menuSwitch);
    
    MenuOption* menuOption = new MenuOption(GetFont(), "BACK", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuOption->callback = std::bind(&Menu::Transition, parentMenu, 0);
    AddMenuOption(menuOption);
    
    //menuOption = new MenuOption(GetFont(), "OPTIONS", glm::vec3(11.f, 3.f, 2.5f), glm::vec3(270.f, 0.f, 330.f), 4.f);
    //AddMenuOption(menuOption);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

glm::vec3 OptionsMenu::GetCameraPosition() const {
    return glm::vec3(-12.f, 0.f, 0.f);
}

glm::vec3 OptionsMenu::GetCameraDirection() const {
    return glm::vec3(90.f, 0.f, 0.f);
}

void OptionsMenu::Fullscreen(std::string option) {
    GameSettings::GetInstance().SetBool("Fullscreen", option == "On");
}

void OptionsMenu::FXAA(std::string option) {
    GameSettings::GetInstance().SetBool("FXAA", option == "On");
}
