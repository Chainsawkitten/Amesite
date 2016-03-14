#include "MenuOption.hpp"

#include <Texture/Texture2D.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include "../Util/Hub.hpp"
#include "../GameObject/Camera.hpp"

MenuOption::MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height) : SuperMenuOption(position, rotation) {
    mPrerenderedText = new Texture2D(font, text);
    mScale = glm::vec2(height * static_cast<float>(mPrerenderedText->GetWidth()) / static_cast<float>(mPrerenderedText->GetHeight()), height);
    callback = std::bind(&MenuOption::EmptyCallback, this);
}

MenuOption::~MenuOption() {
    delete mPrerenderedText;
}

glm::vec2 MenuOption::GetScale() const {
    return mScale;
}

Texture2D* MenuOption::GetTexture() const {
    return mPrerenderedText;
}

void MenuOption::Press() {
    callback();
}

void MenuOption::EmptyCallback() const {
    
}
