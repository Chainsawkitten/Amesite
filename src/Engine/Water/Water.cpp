#include "Water.hpp"

#include "../Resources.hpp"
#include "../Texture/Texture2D.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Geometry/Plane.hpp"

#include "Default3D.vert.hpp"
#include "Water.geom.hpp"
#include "Water.frag.hpp"

Water::Water() {
    // Initialize shaders.
    Shader* vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* geometryShader = Resources().CreateShader(WATER_GEOM, WATER_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    Shader* fragmentShader = Resources().CreateShader(WATER_FRAG, WATER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
    
    mPlane = Resources().CreatePlane();
    
    mWaterTexture = Resources().CreateTexture2DFromFile("Resources/Water.png");
    mDudvMap = Resources().CreateTexture2DFromFile("Resources/WaterDUDV.png");
    mNormalMap = Resources().CreateTexture2DFromFile("Resources/WaterNormal.png");
    mTextureRepeat = glm::vec2(1.f, 1.f);
}

Water::~Water() {
    Resources().FreeTexture2D(mWaterTexture);
    Resources().FreeTexture2D(mDudvMap);
    Resources().FreeTexture2D(mNormalMap);
    
    Resources().FreePlane();
    
    Resources().FreeShaderProgram(mShaderProgram);
}

void Water::SetTextureRepeat(const glm::vec2& textureRepeat) {
    mTextureRepeat = textureRepeat;
}
