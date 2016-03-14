#include "MenuSwitch.hpp"

MenuSwitch::MenuSwitch(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height) : SuperMenuOption(position, rotation) {
    
}

MenuSwitch::~MenuSwitch() {
    
}

glm::vec2 MenuSwitch::GetScale() const {
    return glm::vec2(1.f, 1.f);
}

Texture2D* MenuSwitch::GetTexture() const {
    /// @todo
    return nullptr;
}

void MenuSwitch::Press() {
    /// @todo
}
