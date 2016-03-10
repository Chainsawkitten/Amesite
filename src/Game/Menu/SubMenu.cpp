#include "SubMenu.hpp"

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

SubMenu::SubMenu(Menu* parentMenu) {
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mRotation = glm::vec3(0.f, 0.f, 0.f);
    
    // Load font.
    float fontHeight = glm::ceil(MainWindow::GetInstance()->GetSize().y * 0.07f);
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", fontHeight);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
    // Define menu options.
    mMenuOptions.push_back(new MenuOption(mFont, "START GAME", glm::vec3(0.f, 1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions[0]->callback = std::bind(&Menu::ResumeGame, parentMenu);
    mMenuOptions.push_back(new MenuOption(mFont, "OPTIONS", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions.push_back(new MenuOption(mFont, "QUIT", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f));
    mMenuOptions[2]->callback = std::bind(&SubMenu::Quit, this);
    mSelected = 0;
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
}

SubMenu::~SubMenu() {
    Resources().FreeFont(mFont);
    
    for (MenuOption* menuOption : mMenuOptions)
        delete menuOption;
}

void SubMenu::SetPosition(const glm::vec3& position) {
    mPosition = position;
}

void SubMenu::SetRotation(const glm::vec3& rotation) {
    mRotation = rotation;
}

void SubMenu::Update(const glm::mat4& menuModelMatrix, const glm::vec2& playerScale) {
    Entity* camera = HubInstance().GetMainCamera().body;
    Component::Transform* cameraTransform = camera->GetComponent<Component::Transform>();
    
    // Update model matrix.
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    mModelMatrix = menuModelMatrix * glm::translate(glm::mat4(), mPosition) * orientation;
    
    // Update menu selection.
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

void SubMenu::RenderSelected() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    mMenuOptions[mSelected]->RenderSelected(screenSize, mModelMatrix);
}

void SubMenu::RenderMenuOptions() {
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    for (MenuOption* menuOption : mMenuOptions)
        menuOption->Render(screenSize, mModelMatrix);
}

void SubMenu::Quit() {
    MainWindow::GetInstance()->Close();
}
