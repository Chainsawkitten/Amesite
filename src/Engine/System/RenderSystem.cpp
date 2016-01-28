#include "RenderSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Engine/Shader/Shader.hpp>
#include <Engine/Shader/ShaderProgram.hpp>

#include <Engine/Resources.hpp>
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>

RenderSystem::RenderSystem() {
    mVertShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertShader, mFragShader });
    mShaderProgram->Use();
}

RenderSystem::~RenderSystem() {
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertShader);
    Resources().FreeShader(mFragShader);
}

void RenderSystem::Render(const Scene& scene) {
    Entity* camera = nullptr;

    // Finds camera in scene.
    for (unsigned int i = 0; i < scene.Size() && camera == nullptr; i++) {
        if (scene[i]->GetComponent<Component::Lens>() != nullptr && scene[i]->GetComponent<Component::Transform>() != nullptr) {
            camera = scene[i];
        }
    }

    // Render from camera.
    if (camera != nullptr) {
        glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->GetOrientation()*glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
        glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(glm::vec2(800, 600));

        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);

        // Finds models in scene.
        for (unsigned int i = 0; i < scene.Size(); i++) {
            if (scene[i]->GetComponent<Component::Transform>() != nullptr && scene[i]->GetComponent<Component::Mesh>()) {
                Entity* model = scene[i];
                
                glBindVertexArray(model->GetComponent<Component::Mesh>()->geometry->GetVertexArray());

                // Render model.
                glm::mat4 modelMat = glm::translate(glm::mat4(), model->GetComponent<Component::Transform>()->position) * model->GetComponent<Component::Transform>()->GetOrientation() * glm::scale(glm::mat4(), model->GetComponent<Component::Transform>()->scale);
                glUniformMatrix4fv(mShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);

                glDrawElements(GL_TRIANGLES, model->GetComponent<Component::Mesh>()->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            }
        }
    }
}
