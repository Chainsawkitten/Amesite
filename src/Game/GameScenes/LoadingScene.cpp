#include "LoadingScene.hpp"

#include "../Game.hpp"
#include "MenuScene.hpp"

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>
#include <MainWindow.hpp>

LoadingScene::LoadingScene() {
    mLoadingImage = Resources().CreateTexture2DFromFile("Resources/Loading.png");
    mShown = false;
}

LoadingScene::~LoadingScene() {
    Resources().FreeTexture2D(mLoadingImage);
}

void LoadingScene::Update(float deltaTime) {
    if (mShown)
        Game::GetInstance().SetScene(new MenuScene());
    
    mShown = true;
    
    // Clear screen.
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render logo.
    glm::vec2 size = glm::vec2(static_cast<float>(mLoadingImage->GetWidth()), static_cast<float>(mLoadingImage->GetHeight()));
    glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();
    
    // Scale logo down if larger than screen.
    if (size.x > screenSize.x || size.y > screenSize.y) {
        float xScale = screenSize.x / size.x;
        float yScale = screenSize.y / size.y;
        size *= xScale < yScale ? xScale : yScale;
    }
    
    mLoadingImage->Render((screenSize - size) * 0.5f, size);
}
