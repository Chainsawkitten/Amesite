#include "MenuScene.hpp"

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>
#include <MainWindow.hpp>

MenuScene::MenuScene() {
    mLogo = Resources().CreateTexture2DFromFile("Resources/Mugglorna.png");
}

MenuScene::~MenuScene() {
    Resources().FreeTexture2D(mLogo);
}

void MenuScene::Update(float deltaTime) {
    // Clear screen.
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render logo.
    glm::vec2 size = glm::vec2(static_cast<float>(mLogo->GetWidth()), static_cast<float>(mLogo->GetHeight()));
    glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();
    
    // Scale logo down if larger than screen.
    if (size.x > screenSize.x || size.y > screenSize.y) {
        float xScale = screenSize.x / size.x;
        float yScale = screenSize.y / size.y;
        size *= xScale < yScale ? xScale : yScale;
    }
    
    mLogo->Render((screenSize - size) * 0.5f, size);
}
