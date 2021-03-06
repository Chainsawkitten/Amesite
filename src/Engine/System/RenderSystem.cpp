#include "RenderSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"

#include "../MainWindow.hpp"
#include "../Resources.hpp"
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Animation.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/Material.hpp"
#include "../Texture/Texture2D.hpp"
#include <string>
#include "../Profiling/GPUProfiling.hpp"

#include "../Lighting/DeferredLighting.hpp"
#include "../RenderTarget.hpp"
#include "../Physics/Frustum.hpp"

using namespace System;

RenderSystem::RenderSystem() {
    mVertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mDeferredLighting = new DeferredLighting(MainWindow::GetInstance()->GetSize());
}

RenderSystem::~RenderSystem() {
    delete mDeferredLighting;
    
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
    Resources().FreeShaderProgram(mShaderProgram);
}

void RenderSystem::Render(Scene& scene, RenderTarget* renderTarget, const glm::vec2& screenSize, const glm::vec4& clippingPlane, bool showLightVolumes) {
    mDeferredLighting->SetTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const glm::vec2& windowSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<GLsizei>(windowSize.x), static_cast<GLsizei>(windowSize.y));
    
    mShaderProgram->Use();
    
    Entity* camera = nullptr;
    
    // Find last camera.
    std::vector<Component::Lens*> lenses = scene.GetAll<Component::Lens>();
    for (Component::Lens* lens : lenses) {
        if (lens->entity->GetComponent<Component::Transform>() != nullptr)
            camera = lens->entity;
    };
    
    // Render from camera.
    if (camera != nullptr) {
        { PROFILE_GPU("Render meshes");
            glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->GetWorldCameraOrientation() * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
            glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(windowSize);
            glm::mat4 viewProjectionMat = projectionMat * viewMat;
            
            glUniformMatrix4fv(mShaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMat[0][0]);
            glUniform4fv(mShaderProgram->GetUniformLocation("clippingPlane"), 1, &clippingPlane[0]);
            
            // Finds models in scene.
            std::vector<Component::Mesh*> meshes = scene.GetAll<Component::Mesh>();
            for (Component::Mesh* mesh : meshes) {
                Entity* model = mesh->entity;
                Component::Transform* transform = model->GetComponent<Component::Transform>();
                Component::Material* material = model->GetComponent<Component::Material>();
                if (transform != nullptr && material != nullptr) {
                    glm::mat4 modelMat = transform->modelMatrix;
                    
                    Physics::Frustum frustum(viewProjectionMat * modelMat);
                    if (frustum.Collide(mesh->geometry->GetAxisAlignedBoundingBox())) {
                        glBindVertexArray(mesh->geometry->GetVertexArray());
                        
                        // Set texture locations
                        glUniform1i(mShaderProgram->GetUniformLocation("baseImage"), 0);
                        glUniform1i(mShaderProgram->GetUniformLocation("normalMap"), 1);
                        glUniform1i(mShaderProgram->GetUniformLocation("specularMap"), 2);
                        glUniform1i(mShaderProgram->GetUniformLocation("glowMap"), 3);
                        
                        // Textures
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, material->diffuse->GetTextureID());
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, material->normal->GetTextureID());
                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, material->specular->GetTextureID());
                        glActiveTexture(GL_TEXTURE3);
                        glBindTexture(GL_TEXTURE_2D, material->glow->GetTextureID());
                        
                        // Render model.
                        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);
                        glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
                        glUniformMatrix3fv(mShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
                        
                        glDrawElements(GL_TRIANGLES, mesh->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
                    }
                }
            }
            
            glBindVertexArray(0);
        }
        
        { PROFILE_GPU("Lighting");
            renderTarget->SetTarget();
            //mDeferredLighting->ShowTextures(screenSize);
            glViewport(0, 0, static_cast<GLsizei>(screenSize.x), static_cast<GLsizei>(screenSize.y));
            
            mDeferredLighting->Render(scene, camera, screenSize, showLightVolumes);
        }
    }
}
