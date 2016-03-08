#include "Menu.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
#include <MainWindow.hpp>

Menu::Menu() {
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
}

Menu::~Menu() {
    Resources().FreeFont(mFont);
}

bool Menu::IsActive() const {
    return true;
}

void Menu::Update() {
    /// @todo
}

void Menu::RenderSelected() {
    /// @todo
}

void Menu::RenderMenuOptions() {
    /// @todo
}
