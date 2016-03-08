#include "MenuOption.hpp"

#include <Texture/Texture2D.hpp>
#include <glm/gtc/matrix_transform.hpp>

MenuOption::MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height) {
    prerenderedText = new Texture2D(font, text);
    this->position = position;
    this->rotation = rotation;
    scale = glm::vec2(height * static_cast<float>(prerenderedText->GetWidth()) / static_cast<float>(prerenderedText->GetHeight()), height);
    callback = std::bind(&MenuOption::EmptyCallback, this);
}

MenuOption::~MenuOption() {
    delete prerenderedText;
}

glm::mat4 MenuOption::GetModelMatrix() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    return glm::translate(glm::mat4(), position) * orientation * glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, 1.f));
}

void MenuOption::EmptyCallback() const {
    
}
