#include "Water.hpp"

#include <Engine/Resources.hpp>
#include <Engine/Texture/Texture2D.hpp>
#include <Engine/Shader/Shader.hpp>
#include <Engine/Shader/ShaderProgram.hpp>
#include <Engine/Geometry/Plane.hpp>

#include "Water.vert.hpp"
#include "Water.frag.hpp"

#include "../Util/Hub.hpp"
#include "../GameObject/Camera.hpp"
#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/MainWindow.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <Engine/RenderTarget.hpp>

Water::Water() {
    // Initialize shaders.
    mVertexShader = Resources().CreateShader(WATER_VERT, WATER_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(WATER_FRAG, WATER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mPlane = Resources().CreatePlane();
    
    mWaterTexture = Resources().CreateTexture2DFromFile("Resources/water/Water.png");
    mDudvMap = Resources().CreateTexture2DFromFile("Resources/water/WaterDUDV.png");
    mNormalMap = Resources().CreateTexture2DFromFile("Resources/water/WaterNormal.png");
    mTextureRepeat = glm::vec2(1.f, 1.f);
    
    mTextureOffset = glm::vec2(0.f, 0.f);
    mMoveFactor = 0.f;
    
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mScale = glm::vec3(2000.f, 2000.f, 2000.f);
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    mRefractionTarget = new RenderTarget(screenSize * 0.5f);
    mReflectionTarget = new RenderTarget(screenSize * 0.5f);
}

Water::~Water() {
    Resources().FreeTexture2D(mWaterTexture);
    Resources().FreeTexture2D(mDudvMap);
    Resources().FreeTexture2D(mNormalMap);
    
    delete mRefractionTarget;
    delete mReflectionTarget;
    
    Resources().FreePlane();
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

const glm::vec3& Water::GetPosition() const {
    return mPosition;
}

void Water::SetPosition(const glm::vec3& position) {
    mPosition = position;
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
    glEnablei(GL_BLEND, 0);
    glEnablei(GL_BLEND, 1);
    glBlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunci(1, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set texture locations
    glUniform1i(mShaderProgram->GetUniformLocation("tRefraction"), 0);
    glUniform1i(mShaderProgram->GetUniformLocation("tReflection"), 1);
    glUniform1i(mShaderProgram->GetUniformLocation("tDuDvMap"), 2);
    glUniform1i(mShaderProgram->GetUniformLocation("tWater"), 3);
    glUniform1i(mShaderProgram->GetUniformLocation("tNormalMap"), 4);
    glUniform1i(mShaderProgram->GetUniformLocation("tDepthMap"), 5);
    glUniform1i(mShaderProgram->GetUniformLocation("tRefractionExtra"), 6);
    glUniform1i(mShaderProgram->GetUniformLocation("tReflectionExtra"), 7);
    
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
    
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mRefractionTarget->GetExtraTexture());
    
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, mReflectionTarget->GetExtraTexture());
    
    glUniform2fv(mShaderProgram->GetUniformLocation("screenSize"), 1, &screenSize[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("textureRepeat"), 1, &mTextureRepeat[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("texOffset"), 1, &mTextureOffset[0]);
    glUniform4fv(mShaderProgram->GetUniformLocation("clippingPlane"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.f)[0]);
    glUniform1f(mShaderProgram->GetUniformLocation("moveFactor"), mMoveFactor);
    
    // Send matrices to shader.
    Entity* camera = HubInstance().GetMainCamera().body;
    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
    Component::Lens* cameraLens = camera->GetComponent<Component::Lens>();
    
    glm::mat4 viewMat = cameraTransform->GetWorldCameraOrientation() * glm::translate(glm::mat4(), -cameraTransform->GetWorldPosition());
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
    
    glDisablei(GL_BLEND, 0);
    glDisablei(GL_BLEND, 1);
}

RenderTarget* Water::GetRefractionTarget() const {
    return mRefractionTarget;
}

RenderTarget* Water::GetReflectionTarget() const {
    return mReflectionTarget;
}

glm::vec4 Water::GetRefractionClippingPlane() const {
    /// @todo Don't hardcore clipping planes
    return glm::vec4(0.f, -1.f, 0.f, mPosition.y + 0.01f);
}

glm::vec4 Water::GetReflectionClippingPlane() const {
    /// @todo Don't hardcore clipping planes
    return glm::vec4(0.f, 1.f, 0.f, -mPosition.y + 0.1f);
}

void Water::SetTextureRepeat(const glm::vec2& textureRepeat) {
    mTextureRepeat = textureRepeat;
}

glm::mat4 Water::GetModelMatrix() const {
    return glm::translate(glm::mat4(), mPosition) * glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f)) * glm::scale(glm::mat4(), mScale);
}
