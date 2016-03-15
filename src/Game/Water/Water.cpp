#include "Water.hpp"

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Water.geom.hpp"
#include "Water.frag.hpp"

#include "../Util/Hub.hpp"
#include "../GameObject/Camera.hpp"
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <MainWindow.hpp>

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

void Water::Render() const {
//    refractionTarget->SetSource();
//    reflectionTarget->SetSource();
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    mShaderProgram->Use();
    
    // Blending
    GLboolean blending;
    glGetBooleanv(GL_BLEND, &blending);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set texture locations
    glUniform1i(mShaderProgram->GetUniformLocation("tRefraction"), 0);
    glUniform1i(mShaderProgram->GetUniformLocation("tReflection"), 1);
    glUniform1i(mShaderProgram->GetUniformLocation("tDuDvMap"), 2);
    glUniform1i(mShaderProgram->GetUniformLocation("tWater"), 3);
    glUniform1i(mShaderProgram->GetUniformLocation("tNormalMap"), 4);
    glUniform1i(mShaderProgram->GetUniformLocation("tDepthMap"), 5);
    
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, refractionTarget->ColorTexture());
    
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, reflectionTarget->ColorTexture());
    
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, dudvMap->TextureID());
    
//    glActiveTexture(GL_TEXTURE3);
//    glBindTexture(GL_TEXTURE_2D, waterTexture->TextureID());
    
//    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_2D, normalMap->TextureID());
    
//    glActiveTexture(GL_TEXTURE5);
//    glBindTexture(GL_TEXTURE_2D, refractionTarget->DepthTexture());
    
//    glUniform2fv(mShaderProgram->GetUniformLocation("screenSize"), 1, &screenSize[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("textureRepeat"), 1, &mTextureRepeat[0]);
//    glUniform2fv(mShaderProgram->GetUniformLocation("texOffset"), 1, &texOffset[0]);
    glUniform4fv(mShaderProgram->GetUniformLocation("clippingPlane"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.f)[0]);
//    glUniform1f(mShaderProgram->GetUniformLocation("moveFactor"), moveFactor);
//    glUniform1f(mShaderProgram->GetUniformLocation("zNear"), camera->NearPlane());
//    glUniform1f(mShaderProgram->GetUniformLocation("zFar"), camera->FarPlane());
    
    // Send matrices to shader.
    Entity* camera = HubInstance().GetMainCamera().body;
    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
    
    glm::mat4 view = cameraTransform->modelMatrix;
    glm::mat4 normal = glm::transpose(glm::inverse(view * GetModelMatrix()));
    glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("modelMatrix"), 1, GL_FALSE, &GetModelMatrix()[0][0]);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix3fv(mShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("projectionMatrix"), 1, GL_FALSE, &projectionMat[0][0]);
    
//    glUniform4fv(mShaderProgram->GetUniformLocation("lightPosition"), 1, &(view * light.position)[0]);
//    glUniform3fv(mShaderProgram->GetUniformLocation("lightIntensity"), 1, &light.intensity[0]);
    
    glBindVertexArray(mPlane->GetVertexArray());
    
    glDrawElements(GL_TRIANGLES, mPlane->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    
    if (!blending)
        glDisable(GL_BLEND);
}

void Water::SetTextureRepeat(const glm::vec2& textureRepeat) {
    mTextureRepeat = textureRepeat;
}

glm::mat4 Water::GetModelMatrix() const {
    return glm::mat4();
}
