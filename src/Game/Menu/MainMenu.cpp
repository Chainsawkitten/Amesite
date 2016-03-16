#include "MainMenu.hpp"

#include "MenuOption.hpp"
#include <MainWindow.hpp>
#include "Menu.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
#include <random>
#include <time.h>

MainMenu::MainMenu(Menu* parentMenu) : SubMenu() {
    // Define menu options.
    MenuOption* menuOption = new MenuOption(GetFont(), "START GAME", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 2.f);
    menuOption->callback = std::bind(&Menu::ResumeGame, parentMenu);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "OPTIONS", glm::vec3(0.f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 2.f);
    menuOption->callback = std::bind(&Menu::Transition, parentMenu, 1);
    AddMenuOption(menuOption);
    menuOption = new MenuOption(GetFont(), "QUIT", glm::vec3(0.f, -2.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 2.f);
    menuOption->callback = std::bind(&MainMenu::Quit, this);
    AddMenuOption(menuOption);
    
    // Random subtitle
    std::default_random_engine RNG;
    RNG.seed(time(0));

    std::vector<std::string> subtitleStrings;
    subtitleStrings.push_back("AMETHYST BAR FIGHT");
    subtitleStrings.push_back("FIGHT OR FLIGHT");
    subtitleStrings.push_back("COMPUTER CRYSTAL SHOOTER");
    subtitleStrings.push_back("DARKEST CAVE");
    subtitleStrings.push_back("CAVE ESCAPE");
    subtitleStrings.push_back("DUGOUT DIARY");
    subtitleStrings.push_back("CRYSTAL CAVE");
    subtitleStrings.push_back("CRYSTAL CRASHERS");
    subtitleStrings.push_back("SPACE CANYON 3000");
    subtitleStrings.push_back("CAVESCAPE");
    subtitleStrings.push_back("PORTAL EXPEDITION");

    std::uniform_int_distribution<uint32_t> introStringsDistribution(0, subtitleStrings.size() - 1);

    std::string SubtitleString = subtitleStrings[introStringsDistribution(RNG)];

    // Title.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.18f);
    Font* font = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    font->SetColor(glm::vec3(1.f, 1.f, 1.f));
    AddTitleOption(new MenuOption(font, "AMESITE", glm::vec3(11.f, 1.f, 8.5f), glm::vec3(270.f, 0.f, 314.f), 6.f));

    AddTitleOption(new MenuOption(font, SubtitleString.c_str(), glm::vec3(7.f, -3.f, 7.5f), glm::vec3(270.f, 0.f, 314.f), 2.f));
    Resources().FreeFont(font);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

glm::vec3 MainMenu::GetCameraPosition() const {
    return glm::vec3(-23.f, 4.6f, 23.5f);
}

glm::vec3 MainMenu::GetCameraDirection() const {
    return glm::vec3(60.f, 10.f, 0.f);
}

void MainMenu::Quit() {
    MainWindow::GetInstance()->Close();
}
