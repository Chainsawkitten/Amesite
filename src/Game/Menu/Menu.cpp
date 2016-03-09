#include "Menu.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
#include <MainWindow.hpp>

#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Texture/Texture2D.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include "../Util/Hub.hpp"
#include "../GameObject/Camera.hpp"
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
    Entity* camera = Hub().GetMainCamera().body;
    
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
    
    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
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
        glm::mat4 projectionMat = camera->GetComponent<Component::Lens>()->GetProjection(screenSize);
        glm::vec2 mouseCoordinates(Input()->CursorX(), Input()->CursorY());
        
        glm::vec3 cameraPosition = cameraTransform->position;
        glm::vec3 ray(Picking::CreateWorldRay(mouseCoordinates, viewMat, projectionMat));
        
        for (std::size_t i=0; i < mMenuOptions.size(); ++i) {
            if (mMenuOptions[i]->MouseIntersect(cameraPosition, ray, mModelMatrix, playerScale))
                mSelected = i;
        }
        
        // Handle pressed menu option.
        if (Input()->Triggered(InputHandler::ANYONE, InputHandler::SHOOT))
            mMenuOptions[mSelected]->callback();
    }
}

void Menu::RenderSelected() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    mMenuOptions[mSelected]->RenderSelected(screenSize, mModelMatrix);
}

void Menu::RenderMenuOptions() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    for (MenuOption* menuOption : mMenuOptions)
        menuOption->Render(screenSize, mModelMatrix);
}

void Menu::StartGame() {
    mFlyOut = true;
}

void Menu::Quit() {
    MainWindow::GetInstance()->Close();
}
