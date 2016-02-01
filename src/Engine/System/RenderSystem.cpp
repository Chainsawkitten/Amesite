#include "RenderSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"

#include "../Resources.hpp"
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Mesh.hpp"

using namespace System;

RenderSystem::RenderSystem() {
    mVertShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertShader, mFragShader });
}

RenderSystem::~RenderSystem() {
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertShader);
    Resources().FreeShader(mFragShader);
}

void RenderSystem::Render(const Scene& scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderProgram->Use();
   
    Entity* camera = nullptr;

    // Finds (last) camera in scene.
    for (unsigned int i = 0; i < scene.Size<Component::Lens>(); i++) {
        if (scene.Get<Component::Lens>(i)->entity->GetComponent<Component::Transform>() != nullptr) {
            camera = scene.Get<Component::Lens>(i)->entity;
        }
    }

    // Render from camera.
    if (camera != nullptr) {
        glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->GetOrientation()*glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
        //TODO: DO NOT USE SET VIEWPORT SIZE!
        glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(glm::vec2(800, 600));

        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);

        // Finds models in scene.
        for (unsigned int i = 0; i < scene.Size<Component::Mesh>(); i++) {
            if (scene.Get<Component::Mesh>(i)->entity->GetComponent<Component::Transform>() != nullptr) {
                Entity* model = scene.Get<Component::Mesh>(i)->entity;
                
                glBindVertexArray(model->GetComponent<Component::Mesh>()->geometry->GetVertexArray());

                // Render model.
                glm::mat4 modelMat = model->GetComponent<Component::Transform>()->modelMatrix;
                glUniformMatrix4fv(mShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);

                glDrawElements(GL_TRIANGLES, model->GetComponent<Component::Mesh>()->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            }
        }
    }
}
