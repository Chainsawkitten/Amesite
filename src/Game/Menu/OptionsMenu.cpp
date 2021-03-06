#include "OptionsMenu.hpp"

#include "MenuOption.hpp"
#include "MenuSwitch.hpp"
#include <Engine/MainWindow.hpp>
#include "Menu.hpp"

#include "../Util/GameSettings.hpp"
#include "../Util/Hub.hpp"
#include "../GameObject/Player/Player1.hpp"

#include <Engine/Resources.hpp>
#include <Engine/Font/Font.hpp>

OptionsMenu::OptionsMenu(Menu* parentMenu) : SubMenu() {
    // Define menu options.
    std::vector<std::string> options;
    options.push_back("On");
    options.push_back("Off");
    MenuSwitch* menuSwitch = new MenuSwitch(GetFont(), "Fullscreen", options, glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::Fullscreen, this, std::placeholders::_1);
    menuSwitch->SetSelected(GameSettings::GetInstance().GetBool("Fullscreen") ? 0 : 1);
    AddMenuOption(menuSwitch);

    menuSwitch = new MenuSwitch(GetFont(), "FXAA", options, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::FXAA, this, std::placeholders::_1);
    menuSwitch->SetSelected(GameSettings::GetInstance().GetBool("FXAA") ? 0 : 1);
    AddMenuOption(menuSwitch);
    
    menuSwitch = new MenuSwitch(GetFont(), "Two Players", options, glm::vec3(0.f, 0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::TwoPlayers, this, std::placeholders::_1);
    menuSwitch->SetSelected(GameSettings::GetInstance().GetBool("Two Players") ? 0 : 1);
    AddMenuOption(menuSwitch);
    
    menuSwitch = new MenuSwitch(GetFont(), "Player One Joystick Aim", options, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::MouseAim, this, std::placeholders::_1);
    menuSwitch->SetSelected(GameSettings::GetInstance().GetBool("Player One Joystick Aim") ? 0 : 1);
    AddMenuOption(menuSwitch);
    
    menuSwitch = new MenuSwitch(GetFont(), "Water Reflections", options, glm::vec3(0.f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::WaterReflections, this, std::placeholders::_1);
    menuSwitch->SetSelected(GameSettings::GetInstance().GetBool("Reflections") ? 0 : 1);
    AddMenuOption(menuSwitch);
    
    menuSwitch = new MenuSwitch(GetFont(), "Water Refractions", options, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::WaterRefractions, this, std::placeholders::_1);
    menuSwitch->SetSelected(GameSettings::GetInstance().GetBool("Refractions") ? 0 : 1);
    AddMenuOption(menuSwitch);
    
    std::vector<std::string> volumeOptions;
    volumeOptions.push_back("0");
    volumeOptions.push_back("0.1");
    volumeOptions.push_back("0.2");
    volumeOptions.push_back("0.3");
    volumeOptions.push_back("0.4");
    volumeOptions.push_back("0.5");
    volumeOptions.push_back("0.6");
    volumeOptions.push_back("0.7");
    volumeOptions.push_back("0.8");
    volumeOptions.push_back("0.9");
    volumeOptions.push_back("1");
    menuSwitch = new MenuSwitch(GetFont(), "Volume", volumeOptions, glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuSwitch->callback = std::bind(&OptionsMenu::AudioVolume, this, std::placeholders::_1);
    menuSwitch->SetSelected(static_cast<std::size_t>(GameSettings::GetInstance().GetDouble("Audio Volume") * 10.0 + 0.0001));
    AddMenuOption(menuSwitch);

    MenuOption* menuOption = new MenuOption(GetFont(), "BACK", glm::vec3(0.f, -2.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.35f);
    menuOption->callback = std::bind(&Menu::Transition, parentMenu, 0);
    AddMenuOption(menuOption);
    
    // Title.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.18f);
    Font* font = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    font->SetColor(glm::vec3(1.f, 1.f, 1.f));
    AddTitleOption(new MenuOption(font, "OPTIONS", glm::vec3(-3.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 90.f), 2.f));
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

void OptionsMenu::Fullscreen(std::string option) {
    GameSettings::GetInstance().SetBool("Fullscreen", option == "On");
}

void OptionsMenu::FXAA(std::string option) {
    GameSettings::GetInstance().SetBool("FXAA", option == "On");
}

void OptionsMenu::TwoPlayers(std::string option) {
    GameSettings::GetInstance().SetBool("Two Players", option == "On");
    HubInstance().SetPlayer2State(GameSettings::GetInstance().GetBool("Two Players"));
}

void OptionsMenu::MouseAim (std::string option) {
    GameSettings::GetInstance().SetBool("Player One Joystick Aim", option == "On");
    HubInstance().SetPlayer2State(GameSettings::GetInstance().GetBool("Two Players"));
}

void OptionsMenu::WaterReflections(std::string option) {
    GameSettings::GetInstance().SetBool("Reflections", option == "On");
}

void OptionsMenu::WaterRefractions(std::string option) {
    GameSettings::GetInstance().SetBool("Refractions", option == "On");
}

void OptionsMenu::AudioVolume(std::string option) {
    GameSettings::GetInstance().SetDouble("Audio Volume", std::stod(option));
}
