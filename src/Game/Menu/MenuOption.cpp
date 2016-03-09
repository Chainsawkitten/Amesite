#include "MenuOption.hpp"

#include <Texture/Texture2D.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Util/MainCamera.hpp"
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>

#include <Resources.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Text3D.frag.hpp"
#include "SingleColor3D.frag.hpp"

MenuOption::MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height) {
    prerenderedText = new Texture2D(font, text);
    mPosition = position;
    this->rotation = rotation;
    scale = glm::vec2(height * static_cast<float>(prerenderedText->GetWidth()) / static_cast<float>(prerenderedText->GetHeight()), height);
    callback = std::bind(&MenuOption::EmptyCallback, this);
    
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

MenuOption::~MenuOption() {
    delete prerenderedText;
    
    Resources().FreePlane();
    Resources().FreeShaderProgram(mTextShaderProgram);
    Resources().FreeShaderProgram(mSelectedShaderProgram);
}

glm::mat4 MenuOption::GetModelMatrix() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    return glm::translate(glm::mat4(), mPosition) * orientation * glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, 1.f));
}

bool MenuOption::MouseIntersect(const glm::vec3& cameraPosition, const glm::vec3& ray, const glm::mat4& menuModelMatrix, const glm::vec2& playerScale) {
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
    
    return (fabs(planePosition.x) <= playerScale.x * scale.x * 0.5f && fabs(planePosition.y) <= playerScale.y * scale.y * 0.5f);
}

void MenuOption::RenderSelected(const glm::vec2& screenSize, const glm::mat4& menuModelMatrix) {
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Don't write to the depth buffer.
    GLboolean depthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    glDepthMask(GL_FALSE);
    
    mSelectedShaderProgram->Use();
    
    Entity& camera = MainCamera::GetInstance().GetMainCamera();
    glm::mat4 viewMat = camera.GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera.GetComponent<Component::Transform>()->GetWorldPosition());
    glm::mat4 projectionMat = camera.GetComponent<Component::Lens>()->GetProjection(screenSize);
    
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(mSelectedShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);
    
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

void MenuOption::Render(const glm::vec2& screenSize, const glm::mat4& menuModelMatrix) {
    // Blending enabled.
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    mTextShaderProgram->Use();
    
    Entity& camera = MainCamera::GetInstance().GetMainCamera();
    glm::mat4 viewMat = camera.GetComponent<Component::Transform>()->worldOrientationMatrix * glm::translate(glm::mat4(), -camera.GetComponent<Component::Transform>()->GetWorldPosition());
    glm::mat4 projectionMat = camera.GetComponent<Component::Lens>()->GetProjection(screenSize);
    
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(mTextShaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projectionMat[0][0]);
    
    glBindVertexArray(mPlane->GetVertexArray());
    
    // Texture.
    glUniform1i(mTextShaderProgram->GetUniformLocation("baseImage"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, prerenderedText->GetTextureID());
    
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
}

void MenuOption::EmptyCallback() const {
    
}
