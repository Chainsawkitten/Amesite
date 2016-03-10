#include "SubMenu.hpp"

#include "MenuOption.hpp"
#include <Resources.hpp>
#include <Font/Font.hpp>

#include <MainWindow.hpp>

SubMenu::SubMenu() {
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
    // Define menu options.
    mMenuOptions.push_back(new MenuOption(mFont, "START GAME", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    //mMenuOptions[0]->callback = std::bind(&Menu::StartGame, this);
    mMenuOptions.push_back(new MenuOption(mFont, "OPTIONS", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions.push_back(new MenuOption(mFont, "QUIT", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    //mMenuOptions[2]->callback = std::bind(&Menu::Quit, this);
    mSelected = 0;
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

SubMenu::~SubMenu() {
    Resources().FreeFont(mFont);
    
    for (MenuOption* menuOption : mMenuOptions)
        delete menuOption;
}

void SubMenu::RenderSelected() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    mMenuOptions[mSelected]->RenderSelected(screenSize, mModelMatrix);
}

void SubMenu::RenderMenuOptions() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    for (MenuOption* menuOption : mMenuOptions)
        menuOption->Render(screenSize, mModelMatrix);
}
