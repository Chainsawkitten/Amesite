#include "SubMenu.hpp"

#include "SuperMenuOption.hpp"
#include "MenuOption.hpp"
#include <Resources.hpp>
#include <Font/Font.hpp>

#include "../Util/Hub.hpp"
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include "../GameObject/Camera.hpp"

#include "Menu.hpp"
#include <MainWindow.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Util/Input.hpp>
#include <Util/Picking.hpp>

SubMenu::SubMenu() {
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mRotation = glm::vec3(0.f, 0.f, 0.f);
    
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
    mSelected = 0;
}

SubMenu::~SubMenu() {
    Resources().FreeFont(mFont);
    
    for (SuperMenuOption* menuOption : mMenuOptions)
        delete menuOption;
    
    for (auto titleOption : mTitleOption)
        delete titleOption;
}

void SubMenu::SetPosition(const glm::vec3& position) {
    mPosition = position;
}

const glm::vec3& SubMenu::GetPosition() const {
    return mPosition;
}

void SubMenu::SetRotation(const glm::vec3& rotation) {
    mRotation = rotation;
}

void SubMenu::UpdateModelMatrix(const glm::mat4& menuModelMatrix) {
    // Update model matrix.
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    mModelMatrix = menuModelMatrix * glm::translate(glm::mat4(), mPosition) * orientation;
}

void SubMenu::Update(const glm::vec2& playerScale) {
    Entity* camera = HubInstance().GetMainCamera().body;
    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
    
    // Update menu selection.
    int movement = Input()->Triggered(InputHandler::ANYONE, InputHandler::DOWN) - Input()->Triggered(InputHandler::ANYONE, InputHandler::UP);
    if (mSelected + movement >= 0 && mSelected + movement < static_cast<int>(mMenuOptions.size()))
        mSelected += movement;
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    glm::mat4 viewMat = cameraTransform->GetWorldCameraOrientation() * glm::translate(glm::mat4(), -cameraTransform->GetWorldPosition());
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
        mMenuOptions[mSelected]->Press();
}

void SubMenu::RenderSelected() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    mMenuOptions[mSelected]->RenderSelected(screenSize, mModelMatrix);
}

void SubMenu::RenderMenuOptions() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    for (SuperMenuOption* menuOption : mMenuOptions)
        menuOption->Render(screenSize, mModelMatrix);
    
    for (auto titleOption : mTitleOption)
        titleOption->Render(screenSize, mModelMatrix);
}

void SubMenu::AddMenuOption(SuperMenuOption* menuOption) {
    mMenuOptions.push_back(menuOption);
}

void SubMenu::AddTitleOption(MenuOption* titleOption) {
    mTitleOption.push_back(titleOption);
}

Font* SubMenu::GetFont() const {
    return mFont;
}
