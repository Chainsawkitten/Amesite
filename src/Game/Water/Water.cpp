#include "Water.hpp"

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Water.frag.hpp"

#include "../Util/Hub.hpp"
#include "../GameObject/Camera.hpp"
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <MainWindow.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <RenderTarget.hpp>

Water::Water() {
    // Initialize shaders.
    Shader* vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(WATER_FRAG, WATER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    mPlane = Resources().CreatePlane();
    
    mWaterTexture = Resources().CreateTexture2DFromFile("Resources/Water.png");
    mDudvMap = Resources().CreateTexture2DFromFile("Resources/WaterDUDV.png");
    mNormalMap = Resources().CreateTexture2DFromFile("Resources/WaterNormal.png");
    mTextureRepeat = glm::vec2(1.f, 1.f);
    
    mTextureOffset = glm::vec2(0.f, 0.f);
    mMoveFactor = 0.f;
    
    mPosition = glm::vec3(450.f, 0.f, 450.f);
    mScale = glm::vec3(1000.f, 1000.f, 1000.f);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    mRefractionTarget = new RenderTarget(screenSize);
    mReflectionTarget = new RenderTarget(screenSize);
}

Water::~Water() {
    Resources().FreeTexture2D(mWaterTexture);
    Resources().FreeTexture2D(mDudvMap);
    Resources().FreeTexture2D(mNormalMap);
    
    delete mRefractionTarget;
    delete mReflectionTarget;
    
    Resources().FreePlane();
    
    Resources().FreeShaderProgram(mShaderProgram);
}

void Water::Update(float time, const glm::vec3& wind) {
    mTextureOffset += 0.02f * time * -glm::vec2(wind.x, wind.z);
    mTextureOffset.x = fmod(mTextureOffset.x, 1.f);
    mTextureOffset.y = fmod(mTextureOffset.y, 1.f);
    
    mMoveFactor += time * 0.02f;
    mMoveFactor = fmod(mMoveFactor, 1.f);
}

void Water::Render() const {
    mRefractionTarget->SetSource();
    mReflectionTarget->SetSource();
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    mShaderProgram->Use();
    
    glDisable(GL_CULL_FACE);
    
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
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mRefractionTarget->GetColorTexture());
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mReflectionTarget->GetColorTexture());
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mDudvMap->GetTextureID());
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mWaterTexture->GetTextureID());
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mNormalMap->GetTextureID());
    
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mRefractionTarget->GetDepthTexture());
    
    glUniform2fv(mShaderProgram->GetUniformLocation("screenSize"), 1, &screenSize[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("textureRepeat"), 1, &mTextureRepeat[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("texOffset"), 1, &mTextureOffset[0]);
    glUniform4fv(mShaderProgram->GetUniformLocation("clippingPlane"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.f)[0]);
    glUniform1f(mShaderProgram->GetUniformLocation("moveFactor"), mMoveFactor);
    
    // Send matrices to shader.
    Entity* camera = HubInstance().GetMainCamera().body;
    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
    Component::Lens* cameraLens = camera->GetComponent<Component::Lens>();
    
    glm::mat4 viewMat = cameraTransform->worldOrientationMatrix * glm::translate(glm::mat4(), -cameraTransform->GetWorldPosition());
    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * GetModelMatrix()));
    glm::mat4 projectionMat = cameraLens->GetProjection(screenSize);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &GetModelMatrix()[0][0]);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix3fv(mShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);
    
    glUniform1f(mShaderProgram->GetUniformLocation("zNear"), cameraLens->zNear);
    glUniform1f(mShaderProgram->GetUniformLocation("zFar"), cameraLens->zFar);
    
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
    return glm::translate(glm::mat4(), mPosition) * glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f)) * glm::scale(glm::mat4(), mScale);
}
