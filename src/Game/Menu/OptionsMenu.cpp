#include "OptionsMenu.hpp"

#include "MenuOption.hpp"
#include <MainWindow.hpp>
#include "Menu.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>

OptionsMenu::OptionsMenu(Menu* parentMenu) : SubMenu() {
    // Define menu options.
    MenuOption* menuOption = new MenuOption(GetFont(), "SOME OPTION", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "SOME BULLSHIT", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "BACK", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuOption->callback = std::bind(&Menu::Transition, parentMenu, 0);
    AddMenuOption(menuOption);
    
    // Title.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.18f);
    Font* font = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    font->SetColor(glm::vec3(1.f, 1.f, 1.f));
    SetTitleOption(new MenuOption(font, "OPTIONS", glm::vec3(-3.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 90.f), 2.f));
    Resources().FreeFont(font);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

glm::vec3 OptionsMenu::GetCameraPosition() const {
    return glm::vec3(-12.f, 0.f, 0.f);
}

glm::vec3 OptionsMenu::GetCameraDirection() const {
    return glm::vec3(90.f, 0.f, 0.f);
}
