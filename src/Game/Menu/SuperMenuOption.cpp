#include "SuperMenuOption.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <Texture/Texture2D.hpp>

#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include "../Util/Hub.hpp"
#include "../GameObject/Camera.hpp"

#include <Resources.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Text3D.frag.hpp"
#include "SingleColor3D.frag.hpp"

SuperMenuOption::SuperMenuOption(const glm::vec3& position, const glm::vec3& rotation) {
    mPosition = position;
    mRotation = rotation;
    
    // Initialize shaders.
    mPlane = Resources().CreatePlane();
    Shader* vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(TEXT3D_FRAG, TEXT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mTextShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(fragmentShader);
    
    fragmentShader = Resources().CreateShader(SINGLECOLOR3D_FRAG, SINGLECOLOR3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mSelectedShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
}

SuperMenuOption::~SuperMenuOption() {
    Resources().FreePlane();
    Resources().FreeShaderProgram(mTextShaderProgram);
    Resources().FreeShaderProgram(mSelectedShaderProgram);
}

glm::mat4 SuperMenuOption::GetModelMatrix() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    return glm::translate(glm::mat4(), mPosition) * orientation * glm::scale(glm::mat4(), glm::vec3(GetScale().x, GetScale().y, 1.f));
}

void SuperMenuOption::Press() {
    
}

bool SuperMenuOption::MouseIntersect(const glm::vec3& cameraPosition, const glm::vec3& ray, const glm::mat4& menuModelMatrix, const glm::vec2& playerScale) {
    glm::mat4 modelMatrix(GetModelMatrix());
    
    // Plane vectors.
    glm::mat3 invModelMat(glm::transpose(glm::inverse(menuModelMatrix * modelMatrix)));
    glm::vec3 normal = glm::normalize(invModelMat * glm::vec3(0.f, 0.f, 1.f));
    glm::vec3 tangent = glm::normalize(invModelMat * glm::vec3(1.f, 0.f, 0.f));
    glm::vec3 bitangent = glm::normalize(invModelMat * glm::vec3(0.f, 1.f, 0.f));
    
    // Discard if ray and plane are (almost) parallel.
    float denom = glm::dot(normal, ray);
    if (denom > -1e-6)
        return false;
    
    glm::vec3 origin(menuModelMatrix * glm::vec4(mPosition, 1.f));
    float length = glm::dot(origin - cameraPosition, normal) / denom;
    
    // World position.
    glm::vec3 position(cameraPosition + length * ray);
    
    // Position relative to origin of the plane.
    glm::vec3 q = position - origin;
    
    glm::vec2 planePosition(glm::dot(q, tangent), glm::dot(q, bitangent));
    
    return (fabs(planePosition.x) <= playerScale.x * GetScale().x * 0.5f && fabs(planePosition.y) <= playerScale.y * GetScale().y * 0.5f);
}

void SuperMenuOption::RenderSelected(const glm::vec2& screenSize, const glm::mat4& menuModelMatrix) {
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Don't write to the depth buffer.
    GLboolean depthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    glDepthMask(GL_FALSE);
    
    mSelectedShaderProgram->Use();
    
    Entity* camera = HubInstance().GetMainCamera().body;
    glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
    glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
    glm::mat4 viewProjectionMat = projectionMat * viewMat;
    
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMat[0][0]);
    
    glBindVertexArray(mPlane->GetVertexArray());
    
    glm::mat4 modelMat = menuModelMatrix * GetModelMatrix();
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);
    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
    glUniformMatrix3fv(mSelectedShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
    
    glUniform4fv(mSelectedShaderProgram->GetUniformLocation("color"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.65f)[0]);
    
    glDrawElements(GL_TRIANGLES, mPlane->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    glUseProgram(0);
    
    // Reset depth and blending.
    if (!blend)
        glDisable(GL_BLEND);
    if (depthMask)
        glDepthMask(GL_TRUE);
}

void SuperMenuOption::Render(const glm::vec2& screenSize, const glm::mat4& menuModelMatrix) {
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Disable backface culling.
    GLboolean backfaceCulling = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    
    mTextShaderProgram->Use();
    
    Entity* camera = HubInstance().GetMainCamera().body;
    glm::mat4 viewMat = camera->GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
    glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
    glm::mat4 viewProjectionMat = projectionMat * viewMat;
    
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMat[0][0]);
    
    glBindVertexArray(mPlane->GetVertexArray());
    
    // Texture.
    glUniform1i(mTextShaderProgram->GetUniformLocation("baseImage"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GetTexture()->GetTextureID());
    
    glm::mat4 modelMat = menuModelMatrix * GetModelMatrix();
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &modelMat[0][0]);
    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMat * modelMat));
    glUniformMatrix3fv(mTextShaderProgram->GetUniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normalMat)[0][0]);
    
    glUniform3fv(mTextShaderProgram->GetUniformLocation("color"), 1, &glm::vec3(1.f, 1.f, 1.f)[0]);
    
    glDrawElements(GL_TRIANGLES, mPlane->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    glUseProgram(0);
    
    // Reset blending.
    if (!blend)
        glDisable(GL_BLEND);
    
    if (backfaceCulling)
        glEnable(GL_CULL_FACE);
}
