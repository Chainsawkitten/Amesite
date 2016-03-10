#include "MainMenu.hpp"

#include "MenuOption.hpp"
#include <MainWindow.hpp>
#include "Menu.hpp"

MainMenu::MainMenu(Menu* parentMenu) : SubMenu() {
    // Define menu options.
    MenuOption* menuOption = new MenuOption(GetFont(), "START GAME", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    menuOption->callback = std::bind(&Menu::ResumeGame, parentMenu);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "OPTIONS", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "QUIT", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    menuOption->callback = std::bind(&MainMenu::Quit, this);
    AddMenuOption(menuOption);
    
    //menuOption = new MenuOption(GetFont(), "MODERSHIP", glm::vec3(11.f, 3.f, 2.5f), glm::vec3(270.f, 0.f, 330.f), 4.f);
    //AddMenuOption(menuOption);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

glm::vec3 MainMenu::GetCameraPosition() const {
    return glm::vec3(-3.f, -2.6f, -6.5f);
}

glm::vec3 MainMenu::GetCameraDirection() const {
    return glm::vec3(60.f, 10.f, 0.f);
}

void MainMenu::Quit() {
    MainWindow::GetInstance()->Close();
}
