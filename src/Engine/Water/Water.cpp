#include "Water.hpp"

#include "../Resources.hpp"
#include "../Texture/Texture2D.hpp"

Water::Water() {
    mWaterTexture = Resources().CreateTexture2DFromFile("Resources/Water.png");
    mDudvMap = Resources().CreateTexture2DFromFile("Resources/WaterDUDV.png");
    mNormalMap = Resources().CreateTexture2DFromFile("Resources/WaterNormal.png");
    mTextureRepeat = glm::vec2(1.f, 1.f);
}

Water::~Water() {
    Resources().FreeTexture2D(mWaterTexture);
    Resources().FreeTexture2D(mDudvMap);
    Resources().FreeTexture2D(mNormalMap);
}

void Water::SetTextureRepeat(const glm::vec2& textureRepeat) {
    mTextureRepeat = textureRepeat;
}
