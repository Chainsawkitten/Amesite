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

#include "SubMenu.hpp"
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include <Util/Picking.hpp>

#include <Util/Log.hpp>
#include "../GameObject/Player/SuperPlayer.hpp"

Menu::Menu() {
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mRotation = glm::vec3(0.f, 0.f, 0.f);
    
    mActive = true;
    mFlyOut = false;
    mTimer = 0.f;
    
    // Transition.
    mTransition = false;
    
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
    // Define submenus.
    SubMenu* subMenu = new MainMenu(this);
    subMenu->SetPosition(glm::vec3(0.f, 3.1f, 8.6f));
    subMenu->SetRotation(glm::vec3(0.f, 314.f, 0.f));
    mSubMenus.push_back(subMenu);
    
    subMenu = new OptionsMenu(this);
    subMenu->SetPosition(glm::vec3(-20.2f, 0.f, -4.f));
    subMenu->SetRotation(glm::vec3(270.f, 0.f, 0.f));
    mSubMenus.push_back(subMenu);
    
    mSelected = 0;
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

Menu::~Menu() {
    Resources().FreeFont(mFont);
    
    for (SubMenu* subMenu : mSubMenus)
        delete subMenu;
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
    Entity* camera = HubInstance().GetMainCamera().body;
    
    // Transition.
    glm::vec3 subMenuPosition(mSubMenus[mSelected]->GetPosition() + mSubMenus[mSelected]->GetCameraPosition());
    glm::vec3 subMenuRotation(mSubMenus[mSelected]->GetCameraDirection());
    if (mTransition) {
        mTransitionTimer += deltaTime;
        if (mTransitionTimer > 1.f) {
            mTransitionTimer = 1.f;
            mTransition = false;
            mSelected = mNextSubMenu;
        }
        
        glm::vec3 newPosition(mSubMenus[mNextSubMenu]->GetPosition() + mSubMenus[mNextSubMenu]->GetCameraPosition());
        glm::vec3 newRotation(mSubMenus[mNextSubMenu]->GetCameraDirection());
        subMenuPosition = (1.f - mTransitionTimer) * subMenuPosition + mTransitionTimer * newPosition;
        subMenuRotation = (1.f - mTransitionTimer) * subMenuRotation + mTransitionTimer * newRotation;
    }
    
    Component::Transform* playerTransform = player->GetNodeEntity()->GetComponent<Component::Transform>();
    glm::mat4 playerModelMatrix(playerTransform->modelMatrix);
    subMenuPosition = glm::vec3(playerModelMatrix * glm::vec4(subMenuPosition, 1.f));
    
    // Fly out camera.
    float weight = 1.f;
    if (mFlyOut) {
        mTimer += deltaTime;
        if (mTimer > 1.f) {
            mTimer = 1.f;
            mActive = false;
        }
        
        weight = 1.f - mTimer;
    }

    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
    cameraTransform->position = (1.f - weight) * cameraTransform->GetWorldPosition() + weight * subMenuPosition;
    cameraTransform->yaw = (1.f - weight) * cameraTransform->yaw + weight * subMenuRotation.x - playerTransform->yaw;
    cameraTransform->pitch = (1.f - weight) * cameraTransform->pitch + weight * subMenuRotation.y;
    cameraTransform->roll = (1.f - weight) * cameraTransform->roll + weight * subMenuRotation.z;
    cameraTransform->UpdateModelMatrix();
    
    // Update model matrix.
    glm::vec2 playerScale(playerTransform->scale.x, playerTransform->scale.z);
    
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    mModelMatrix = playerModelMatrix * glm::translate(glm::mat4(), mPosition) * orientation;
    
    if (!mTransition) {
        for (SubMenu* subMenu : mSubMenus)
            subMenu->UpdateModelMatrix(mModelMatrix);
        
        // Update menu selection.
        if (!mFlyOut) {
            mSubMenus[mSelected]->Update(playerScale);
        }
    }
}

void Menu::RenderSelected() {
    mSubMenus[mSelected]->RenderSelected();
}

void Menu::RenderMenuOptions() {
    for (SubMenu* subMenu : mSubMenus)
        subMenu->RenderMenuOptions();
}

void Menu::PauseGame() {
    mActive = true;
    mFlyOut = false;
}

void Menu::ResumeGame() {
    mFlyOut = true;
}

void Menu::Transition(int subMenuIndex) {
    mTransition = true;
    mNextSubMenu = subMenuIndex;
    mTransitionTimer = 0.f;
}
