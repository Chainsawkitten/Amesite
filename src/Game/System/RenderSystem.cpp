#include "RenderSystem.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <MainWindow.hpp>

#include <Engine/Geometry/Cube.hpp>
#include <Engine/Shader/Shader.hpp>
#include <Engine/Shader/ShaderProgram.hpp>

#include <Resources.hpp>
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>

RenderSystem::RenderSystem(ShaderProgram* shaderProgram) {
    Init(shaderProgram);
}

RenderSystem::~RenderSystem() {

}

void RenderSystem::Init(ShaderProgram* shaderProgram) {
    mShaderProgram = shaderProgram;
}

void RenderSystem::Render(Scene* scene) {
    Entity* camera = nullptr;

    // Finds camera in scene.
    for (unsigned int i = 0; i < scene->Size() && camera == nullptr; i++) {
        if ((*scene)[i]->GetComponent<Component::Lens>() != nullptr && (*scene)[i]->GetComponent<Component::Transform>() != nullptr) {
            camera = (*scene)[i];
        }
    }

    // Render from camera.
    if (camera != nullptr) {
        glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->GetOrientation()*glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
        glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(glm::vec2(800, 600));

        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
        glUniformMatrix4fv(mShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);

        // Finds models in scene.
        for (unsigned int i = 0; i < scene->Size(); i++) {
            if ((*scene)[i]->GetComponent<Component::Transform>() != nullptr && (*scene)[i]->GetComponent<Component::Mesh>()) {
                Entity* model = (*scene)[i];
                
                glBindVertexArray(model->GetComponent<Component::Mesh>()->geometry->GetVertexArray());

                // Render model.
                glm::mat4 modelMat = glm::translate(glm::mat4(), model->GetComponent<Component::Transform>()->position) * model->GetComponent<Component::Transform>()->GetOrientation() * glm::scale(glm::mat4(), model->GetComponent<Component::Transform>()->scale);
                glUniformMatrix4fv(mShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);

                glDrawElements(GL_TRIANGLES, model->GetComponent<Component::Mesh>()->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            }
        }
    }
}
