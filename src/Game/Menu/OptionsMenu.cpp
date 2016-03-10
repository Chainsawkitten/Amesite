#include "OptionsMenu.hpp"

#include "MenuOption.hpp"
#include <MainWindow.hpp>
#include "Menu.hpp"

OptionsMenu::OptionsMenu(Menu* parentMenu) : SubMenu() {
    // Define menu options.
    MenuOption* menuOption = new MenuOption(GetFont(), "SOME OPTION", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "SOME BULLSHIT", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "BACK", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    menuOption->callback = std::bind(&Menu::Transition, parentMenu, 0);
    AddMenuOption(menuOption);
    
    //menuOption = new MenuOption(GetFont(), "OPTIONS", glm::vec3(11.f, 3.f, 2.5f), glm::vec3(270.f, 0.f, 330.f), 4.f);
    //AddMenuOption(menuOption);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

glm::vec3 OptionsMenu::GetCameraPosition() const {
    return glm::vec3(2.f, 0.f, 0.f);
}

glm::vec3 OptionsMenu::GetCameraDirection() const {
    return glm::vec3(90.f, 0.f, 0.f);
}
