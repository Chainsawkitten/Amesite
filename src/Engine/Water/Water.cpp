#include "Water.hpp"

#include "../Resources.hpp"
#include "../Texture/Texture2D.hpp"

Water::Water() {
    mWaterTexture = Resources().CreateTexture2DFromFile("Resources/Water.png");
    mDudvMap = Resources().CreateTexture2DFromFile("Resources/WaterDUDV.png");
    mNormalMap = Resources().CreateTexture2DFromFile("Resources/WaterNormal.png");
}

Water::~Water() {
    Resources().FreeTexture2D(mWaterTexture);
    Resources().FreeTexture2D(mDudvMap);
    Resources().FreeTexture2D(mNormalMap);
}
