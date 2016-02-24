#include "MenuScene.hpp"

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>
#include <MainWindow.hpp>

#include <PostProcessing/PostProcessing.hpp>
#include <PostProcessing/FXAAFilter.hpp>
#include <PostProcessing/GammaCorrectionFilter.hpp>
#include <PostProcessing/GlowFilter.hpp>
#include <PostProcessing/GlowBlurFilter.hpp>
#include "../Util/GameSettings.hpp"

#include "../Util/GameEntityFactory.hpp"
#include "../Util/MainCamera.hpp"
#include "../GameObject/Camera.hpp"

MenuScene::MenuScene() {
    mLogo = Resources().CreateTexture2DFromFile("Resources/Mugglorna.png");
    
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);
    
    // Create main camera
    mMainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 70.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    MainCameraInstance().SetMainCamera(mMainCamera->body);
    
    GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    
    // Initialize post-processing.
    postProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    fxaaFilter = new FXAAFilter();
    gammaCorrectionFilter = new GammaCorrectionFilter();
    glowFilter = new GlowFilter();
    glowBlurFilter = new GlowBlurFilter();
}

MenuScene::~MenuScene() {
    Resources().FreeTexture2D(mLogo);
    
    delete fxaaFilter;
    delete gammaCorrectionFilter;
    delete glowFilter;
    delete glowBlurFilter;
    delete postProcessing;
}

void MenuScene::Update(float deltaTime) {
    // Render.
    mRenderSystem.Render(*this, postProcessing->GetRenderTarget());
    
    // Glow.
    glowBlurFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
    int blurAmount = 5;
    for (int i=0; i<blurAmount; ++i) {
        glowBlurFilter->SetHorizontal(true);
        postProcessing->ApplyFilter(glowBlurFilter);
        glowBlurFilter->SetHorizontal(false);
        postProcessing->ApplyFilter(glowBlurFilter);
    }
    postProcessing->ApplyFilter(glowFilter);
    
    // Anti-aliasing.
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        fxaaFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
        postProcessing->ApplyFilter(fxaaFilter);
    }
    
    // Gamma correction.
    gammaCorrectionFilter->SetBrightness((float)GameSettings::GetInstance().GetDouble("Gamma"));
    postProcessing->ApplyFilter(gammaCorrectionFilter);
    
    // Render to back buffer.
    postProcessing->Render();
    
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
