#include "DeferredLighting.hpp"
#include "../Util/Log.hpp"

#include "../Resources.hpp"
#include "../Geometry/Square.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "Deferred.frag.hpp"

#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/DirectionalLight.hpp"
#include "../Component/PointLight.hpp"
#include "../Component/SpotLight.hpp"
#include "../Component/Animation.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "../Physics/AxisAlignedBoundingBox.hpp"
#include "../Physics/Frustum.hpp"

#include "../Profiling/CPUProfiling.hpp"
#include <string>

DeferredLighting::DeferredLighting(const glm::vec2& size) {
    mSize = size;
    
    mVertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(DEFERRED_FRAG, DEFERRED_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mSquare = Resources().CreateSquare();
    
    // Create the FBO
    glGenFramebuffers(1, &mFrameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
    
    // Generate textures
    glGenTextures(NUM_TEXTURES, mTextures);
    glGenTextures(1, &mDepthHandle);
    
    unsigned int width = static_cast<unsigned int>(size.x);
    unsigned int height = static_cast<unsigned int>(size.y);
    AttachTexture(mTextures[DIFFUSE], width, height, GL_COLOR_ATTACHMENT0 + DIFFUSE, GL_RGB16F);
    AttachTexture(mTextures[NORMAL], width, height, GL_COLOR_ATTACHMENT0 + NORMAL, GL_RGB16F);
    AttachTexture(mTextures[SPECULAR], width, height, GL_COLOR_ATTACHMENT0 + SPECULAR, GL_RGB);
    AttachTexture(mTextures[GLOW], width, height, GL_COLOR_ATTACHMENT0 + GLOW, GL_RGB);
    
    // Bind depthHandle
    glBindTexture(GL_TEXTURE_2D, mDepthHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthHandle, 0);
    
    // Create and intialize draw buffers (output from geometry pass)
    GLenum drawBuffers[NUM_TEXTURES];
    for (unsigned int i = 0; i < NUM_TEXTURES; i++)
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    glDrawBuffers(NUM_TEXTURES, drawBuffers);
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Frame buffer creation failed\n";
    
    // Default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    // Store light uniform locations.
    for (unsigned int lightIndex = 0; lightIndex < mLightCount; ++lightIndex) {
        mLightUniforms[lightIndex].position = mShaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].position").c_str());
        mLightUniforms[lightIndex].intensities = mShaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].intensities").c_str());
        mLightUniforms[lightIndex].attenuation = mShaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].attenuation").c_str());
        mLightUniforms[lightIndex].ambientCoefficient = mShaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].ambientCoefficient").c_str());
        mLightUniforms[lightIndex].coneAngle = mShaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].coneAngle").c_str());
        mLightUniforms[lightIndex].direction = mShaderProgram->GetUniformLocation(("lights[" + std::to_string(lightIndex) + "].direction").c_str());
    }
}

DeferredLighting::~DeferredLighting() {
    if (mFrameBufferObject != 0)
        glDeleteFramebuffers(1, &mFrameBufferObject);
    
    if (mTextures[0] != 0)
        glDeleteTextures(NUM_TEXTURES, mTextures);
    
    if (mDepthHandle != 0)
        glDeleteTextures(1, &mDepthHandle);
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
    
    Resources().FreeSquare();
}

void DeferredLighting::SetTarget() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBufferObject);
}

void DeferredLighting::ResetTarget() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void DeferredLighting::ShowTextures(const glm::vec2& size) {
    // Disable depth testing
    glDisable(GL_DEPTH_TEST);
    
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    BindForTexReading();
    
    SetReadBuffer(DeferredLighting::DIFFUSE);
    glBlitFramebuffer(0, 0, width, height, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    SetReadBuffer(DeferredLighting::NORMAL);
    glBlitFramebuffer(0, 0, width, height, 0, halfHeight, halfWidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    SetReadBuffer(DeferredLighting::SPECULAR);
    glBlitFramebuffer(0, 0, width, height, halfWidth, halfHeight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    SetReadBuffer(DeferredLighting::GLOW);
    glBlitFramebuffer(0, 0, width, height, halfWidth, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    glEnable(GL_DEPTH_TEST);
}

void DeferredLighting::Render(Scene& scene, Entity* camera, const glm::vec2& screenSize) {
    // Set depth testing to always pass.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    
    // Blending enabled for handling multiple light sources
    glEnablei(GL_BLEND, 0);
    glBlendEquationi(0, GL_FUNC_ADD);
    glBlendFunci(0, GL_ONE, GL_ONE);
    
    mShaderProgram->Use();
    
    BindForReading();
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(mSquare->GetVertexArray());
    
    unsigned int renderedLights = 0;
    
    // Set uniforms.
    glUniform1i(mShaderProgram->GetUniformLocation("tDiffuse"), DeferredLighting::DIFFUSE);
    glUniform1i(mShaderProgram->GetUniformLocation("tNormals"), DeferredLighting::NORMAL);
    glUniform1i(mShaderProgram->GetUniformLocation("tSpecular"), DeferredLighting::SPECULAR);
    glUniform1i(mShaderProgram->GetUniformLocation("tGlow"), DeferredLighting::GLOW);
    glUniform1i(mShaderProgram->GetUniformLocation("tDepth"), DeferredLighting::NUM_TEXTURES);
    glUniform1i(mShaderProgram->GetUniformLocation("lightCount"), mLightCount);
    
    // Get the camera matrices.
    glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->GetWorldCameraOrientation() * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
    glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
    glm::mat4 viewProjectionMat(projectionMat * viewMat);
    
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("inverseProjectionMatrix"), 1, GL_FALSE, &glm::inverse(projectionMat)[0][0]);
    
    unsigned int lightIndex = 0U;
    
    // Render all directional lights.
    std::vector<Component::DirectionalLight*>& directionalLights = scene.GetAll<Component::DirectionalLight>();
    for (Component::DirectionalLight* light : directionalLights) {
        Entity* lightEntity = light->entity;
        Component::Transform* transform = lightEntity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            glm::vec4 direction = glm::vec4(transform->GetWorldDirection(), 0.f);
            glUniform4fv(mLightUniforms[lightIndex].position, 1, &(viewMat * -direction)[0]);
            glUniform3fv(mLightUniforms[lightIndex].intensities, 1, &light->color[0]);
            glUniform1f(mLightUniforms[lightIndex].attenuation, 1.f);
            glUniform1f(mLightUniforms[lightIndex].ambientCoefficient, light->ambientCoefficient);
            glUniform1f(mLightUniforms[lightIndex].coneAngle, 0.f);
            glUniform3fv(mLightUniforms[lightIndex].direction, 1, &glm::vec3(0.f, 0.f, 0.f)[0]);
            
            ++renderedLights;
            if (++lightIndex >= mLightCount) {
                lightIndex = 0U;
                glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            }
        }
    }
    
    // Render all spot lights.
    std::vector<Component::SpotLight*>& spotLights = scene.GetAll<Component::SpotLight>();
    for (Component::SpotLight* light : spotLights) {
        Entity* lightEntity = light->entity;
        Component::Transform* transform = lightEntity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            glm::vec4 direction = viewMat * glm::vec4(transform->GetWorldDirection(), 0.f);
            glUniform4fv(mLightUniforms[lightIndex].position, 1, &(viewMat * (glm::vec4(glm::vec3(transform->modelMatrix[3][0], transform->modelMatrix[3][1], transform->modelMatrix[3][2]), 1.0)))[0]);
            glUniform3fv(mLightUniforms[lightIndex].intensities, 1, &(light->color * light->intensity)[0]);
            glUniform1f(mLightUniforms[lightIndex].attenuation, light->attenuation);
            glUniform1f(mLightUniforms[lightIndex].ambientCoefficient, light->ambientCoefficient);
            glUniform1f(mLightUniforms[lightIndex].coneAngle, light->coneAngle);
            glUniform3fv(mLightUniforms[lightIndex].direction, 1, &glm::vec3(direction)[0]);
            
            ++renderedLights;
            if (++lightIndex >= mLightCount) {
                lightIndex = 0U;
                glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            }
        }
    }
    
    // At which point lights should be cut off (no longer contribute).
    double cutOff = 0.0001;
    
    Physics::AxisAlignedBoundingBox aabb(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Render all point lights.
    std::vector<Component::PointLight*>& pointLights = scene.GetAll<Component::PointLight>();
    for (Component::PointLight* light : pointLights) {
        Entity* lightEntity = light->entity;
        Component::Transform* transform = lightEntity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            float scale = sqrt((1.0 / cutOff - 1.0) / light->attenuation);
            glm::mat4 modelMat = glm::translate(glm::mat4(), transform->GetWorldPosition()) * glm::scale(glm::mat4(), glm::vec3(1.f, 1.f, 1.f) * scale);
            
            Physics::Frustum frustum(viewProjectionMat * modelMat);
            if (frustum.Collide(aabb)) {
                glUniform4fv(mLightUniforms[lightIndex].position, 1, &(viewMat * (glm::vec4(glm::vec3(transform->modelMatrix[3][0], transform->modelMatrix[3][1], transform->modelMatrix[3][2]), 1.0)))[0]);
                glUniform3fv(mLightUniforms[lightIndex].intensities, 1, &(light->color * light->intensity)[0]);
                glUniform1f(mLightUniforms[lightIndex].attenuation, light->attenuation);
                glUniform1f(mLightUniforms[lightIndex].ambientCoefficient, light->ambientCoefficient);
                glUniform1f(mLightUniforms[lightIndex].coneAngle, 180.f);
                glUniform3fv(mLightUniforms[lightIndex].direction, 1, &glm::vec3(1.f, 0.f, 0.f)[0]);
                
                ++renderedLights;
                if (++lightIndex >= mLightCount) {
                    lightIndex = 0U;
                    glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
                }
            }
        }
    }
    
    if (lightIndex != 0U) {
        glUniform1i(mShaderProgram->GetUniformLocation("lightCount"), lightIndex);
        glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
    
    { PROFILE_CPU((std::to_string(renderedLights) + " lights").c_str());
    }
    
    // Reset blending and depth function to standard values.
    glDisablei(GL_BLEND, 0);
    glDepthFunc(GL_LESS);
}

void DeferredLighting::AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
}

void DeferredLighting::BindForReading() {
    for (unsigned int i = 0; i < NUM_TEXTURES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]);
    }
    
    glActiveTexture(GL_TEXTURE0 + NUM_TEXTURES);
    glBindTexture(GL_TEXTURE_2D, mDepthHandle);
}

void DeferredLighting::BindForTexReading() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBufferObject);
}

void DeferredLighting::SetReadBuffer(TEXTURE_TYPE textureType){
    glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}
