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

#include "../Lighting/DeferredLighting.hpp"
#include "../RenderTarget.hpp"

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

void RenderSystem::Render(Scene& scene, RenderTarget* renderTarget) {
    glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();
    
    mDeferredLighting->SetTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderProgram->Use();
    
    Entity* camera = nullptr;
    
    // Find last camera.
    std::vector<Component::Lens*> lenses = scene.GetAll<Component::Lens>();
    for (unsigned int i = 0; i < lenses.size(); i++) {
        if (lenses[i]->entity->GetComponent<Component::Transform>() != nullptr)
            camera = lenses[i]->entity;
    };
    
    // Render from camera.
    if (camera != nullptr) {
        glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
        glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
        
        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);
        
        // Finds models in scene.
        std::vector<Component::Mesh*> meshes = scene.GetAll<Component::Mesh>();
        for (unsigned int i = 0; i < meshes.size(); i++) {
            Entity* model = meshes[i]->entity;
            Component::Transform* transform = model->GetComponent<Component::Transform>();
            Component::Material* material = model->GetComponent<Component::Material>();
            if (transform != nullptr && material != nullptr) {
                glBindVertexArray(meshes[i]->geometry->GetVertexArray());
                
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
                glm::mat4 modelMat = transform->modelMatrix;
                glUniformMatrix4fv(mShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);
                glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
                glUniformMatrix3fv(mShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
                
                glDrawElements(GL_TRIANGLES, meshes[i]->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            }
        }
        
        glBindVertexArray(0);

        renderTarget->SetTarget();
        //mDeferredLighting->ShowTextures(screenSize);
        mDeferredLighting->Render(scene, camera, screenSize);
        
        // Render the particle system
        particleRenderSystem.Render(scene, camera, screenSize);
    }
}
