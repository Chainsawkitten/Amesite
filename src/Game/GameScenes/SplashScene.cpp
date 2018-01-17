#include "SplashScene.hpp"

#include <Engine/Resources.hpp>
#include <Engine/Texture/Texture2D.hpp>
#include <Engine/MainWindow.hpp>
#include "../Game.hpp"
#include "LoadingScene.hpp"

SplashScene::SplashScene() {
    mLogo = Resources().CreateTexture2DFromFile("Resources/Mugglorna.png");
    mElapsedTime = 0.f;
}

SplashScene::~SplashScene() {
    Resources().FreeTexture2D(mLogo);
}

void SplashScene::Update(float deltaTime) {
    mElapsedTime += deltaTime;
    
    // Go to main scene when splash screen is over.
    if (mElapsedTime > 4.5f)
        Game::GetInstance().SetScene(new LoadingScene());
    
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
    
    // Fade in and out logo.
    float alpha = 0.5f;
    if (mElapsedTime > 3.5f) {
        alpha = 4.f - mElapsedTime;
    } else if (mElapsedTime > 0.5f) {
        alpha = 1.f;
    } else {
        alpha = mElapsedTime * 2.f;
    }
    
    mLogo->Render((screenSize - size) * 0.5f, size, alpha);
}
