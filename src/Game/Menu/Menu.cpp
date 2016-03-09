#include "Menu.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
#include <MainWindow.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Geometry/Plane.hpp>

#include "Default3D.vert.hpp"
#include "Text3D.frag.hpp"
#include "SingleColor3D.frag.hpp"

#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Texture/Texture2D.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include "../Util/MainCamera.hpp"
#include <Util/Input.hpp>

#include "MenuOption.hpp"
#include <Util/Picking.hpp>

#include <Util/Log.hpp>
#include "../GameObject/Player/SuperPlayer.hpp"

Menu::Menu() {
    mActive = true;
    mFlyOut = false;
    mTimer = 0.f;
    
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
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
    
    // Define menu options.
    mMenuOptions.push_back(new MenuOption(mFont, "START GAME", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions[0]->callback = std::bind(&Menu::StartGame, this);
    mMenuOptions.push_back(new MenuOption(mFont, "OPTIONS", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions.push_back(new MenuOption(mFont, "QUIT", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions[2]->callback = std::bind(&Menu::Quit, this);
    mSelected = 0;
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

Menu::~Menu() {
    Resources().FreeFont(mFont);
    Resources().FreePlane();
    Resources().FreeShaderProgram(mTextShaderProgram);
    Resources().FreeShaderProgram(mSelectedShaderProgram);
    
    for (MenuOption* menuOption : mMenuOptions) {
        delete menuOption;
    }
}

bool Menu::IsActive() const {
    return mActive;
}

void Menu::SetPosition(const glm::vec3& position) {
    mPosition = position;
}

void Menu::SetRotation(const glm::vec3& rotation) {
    mRotation = rotation;
}

void Menu::Update(GameObject::SuperPlayer* player, float deltaTime) {
    Entity& camera = MainCamera::GetInstance().GetMainCamera();
    
    float weight = 1.f;
    
    // Fly out camera.
    if (mFlyOut) {
        mTimer += deltaTime;
        if (mTimer > 1.f) {
            mTimer = 1.f;
            mActive = false;
        }
        
        weight = 1.f - mTimer;
    }
    
    Component::Transform* cameraTransform = camera.GetComponent<Component::Transform>();
    cameraTransform->position = (1.f - weight) * cameraTransform->position + weight * (player->GetPosition() + glm::vec3(-3.f, 1.4f, 5.f));
    cameraTransform->yaw = (1.f - weight) * cameraTransform->yaw + weight * 60.f;
    cameraTransform->pitch = (1.f - weight) * cameraTransform->pitch + weight * 10.f;
    cameraTransform->roll = (1.f - weight) * cameraTransform->roll + weight * 0.f;
    cameraTransform->UpdateModelMatrix();
    
    // Update model matrix.
    Component::Transform* playerTransform = player->GetNodeEntity()->GetComponent<Component::Transform>();
    glm::mat4 playerModelMatrix(playerTransform->modelMatrix);
    glm::vec2 playerScale(playerTransform->scale.x, playerTransform->scale.z);
    
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    mModelMatrix = playerModelMatrix * glm::translate(glm::mat4(), mPosition) * orientation;
    
    // Update menu selection.
    if (!mFlyOut) {
        int movement = Input()->Triggered(InputHandler::ANYONE, InputHandler::DOWN) - Input()->Triggered(InputHandler::ANYONE, InputHandler::UP);
        if (mSelected + movement >= 0 && mSelected + movement < static_cast<int>(mMenuOptions.size()))
            mSelected += movement;
        
        const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
        
        glm::mat4 viewMat = cameraTransform->worldOrientationMatrix * glm::translate(glm::mat4(), -cameraTransform->GetWorldPosition());
        glm::mat4 projectionMat = camera.GetComponent<Component::Lens>()->GetProjection(screenSize);
        glm::vec2 mouseCoordinates(Input()->CursorX(), Input()->CursorY());
        
        glm::vec3 cameraPosition = cameraTransform->position;
        glm::vec3 ray(Picking::CreateWorldRay(mouseCoordinates, viewMat, projectionMat));
        
        for (std::size_t i=0; i < mMenuOptions.size(); ++i) {
            // Plane vectors.
            glm::mat3 invModelMat(glm::transpose(glm::inverse(mModelMatrix * mMenuOptions[i]->GetModelMatrix())));
            glm::vec3 normal = glm::normalize(invModelMat * glm::vec3(0.f, 0.f, 1.f));
            glm::vec3 tangent = glm::normalize(invModelMat * glm::vec3(1.f, 0.f, 0.f));
            glm::vec3 bitangent = glm::normalize(invModelMat * glm::vec3(0.f, 1.f, 0.f));
            
            // Discard if ray and plane are (almost) parallel.
            float denom = glm::dot(normal, ray);
            if (denom > -1e-6)
                continue;
            
            glm::vec3 origin(mModelMatrix * glm::vec4(mMenuOptions[i]->position, 1.f));
            float length = glm::dot(origin - cameraPosition, normal) / denom;
            
            // World position.
            glm::vec3 position(cameraPosition + length * ray);
            
            // Position relative to origin of the plane.
            glm::vec3 q = position - origin;
            
            glm::vec2 planePosition(glm::dot(q, tangent), glm::dot(q, bitangent));
            
            if (fabs(planePosition.x) <= playerScale.x * mMenuOptions[i]->scale.x * 0.5f && fabs(planePosition.y) <= playerScale.y * mMenuOptions[i]->scale.y * 0.5f)
                mSelected = i;
        }
        
        // Handle pressed menu option.
        if (Input()->Triggered(InputHandler::ANYONE, InputHandler::SHOOT))
            mMenuOptions[mSelected]->callback();
    }
}

void Menu::RenderSelected() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    RenderSelectedMenuOption(mMenuOptions[mSelected], screenSize, mModelMatrix);
}

void Menu::RenderMenuOptions() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    for (MenuOption* menuOption : mMenuOptions)
        RenderMenuOption(menuOption, screenSize, mModelMatrix);
}

void Menu::RenderSelectedMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize, const glm::mat4& menuModelMatrix) {
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
    
    glm::mat4 modelMat = menuModelMatrix * menuOption->GetModelMatrix();
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

void Menu::RenderMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize, const glm::mat4& menuModelMatrix) {
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
    glBindTexture(GL_TEXTURE_2D, menuOption->prerenderedText->GetTextureID());
    
    glm::mat4 modelMat = menuModelMatrix * menuOption->GetModelMatrix();
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

void Menu::StartGame() {
    mFlyOut = true;
}

void Menu::Quit() {
    MainWindow::GetInstance()->Close();
}
