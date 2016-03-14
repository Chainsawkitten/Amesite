#include "MenuSwitch.hpp"

#include <Texture/Texture2D.hpp>

MenuSwitch::MenuSwitch(Font* font, const std::string& text, std::vector<std::string>& options, const glm::vec3& position, const glm::vec3& rotation, float height) : SuperMenuOption(position, rotation) {
    mOptions = options;
    mSelected = 0;
    mHeight = height;
    
    mPrerenderedTexts = new Texture2D*[mOptions.size()];
    for (std::size_t i=0; i<mOptions.size(); ++i)
        mPrerenderedTexts[i] = new Texture2D(font, (text + ": " + mOptions[i]).c_str());
}

MenuSwitch::~MenuSwitch() {
    for (std::size_t i=0; i<mOptions.size(); ++i)
        delete mPrerenderedTexts[i];
    
    delete[] mPrerenderedTexts;
}

glm::vec2 MenuSwitch::GetScale() const {
    return glm::vec2(mHeight * static_cast<float>(mPrerenderedTexts[mSelected]->GetWidth()) / static_cast<float>(mPrerenderedTexts[mSelected]->GetHeight()), mHeight);
}

Texture2D* MenuSwitch::GetTexture() const {
    return mPrerenderedTexts[mSelected];
}

void MenuSwitch::Press() {
    mSelected = (mSelected + 1) % mOptions.size();
}
