#include "MenuScene.hpp"

#include <Resources.hpp>
#include <Font/Font.hpp>
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
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/DirectionalLight.hpp>

MenuScene::MenuScene() {
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);
    
    // Create main camera
    mMainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 0.f, 0.f));
    MainCameraInstance().SetMainCamera(mMainCamera->body);
    
    GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    
    // Directional light.
    Entity* dirLight = CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.01f, 0.01f, 0.01f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.2f;
    
    // Initialize post-processing.
    mPostProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    mFxaaFilter = new FXAAFilter();
    mGammaCorrectionFilter = new GammaCorrectionFilter();
    mGlowFilter = new GlowFilter();
    mGlowBlurFilter = new GlowBlurFilter();
    
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", 20.f);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));
    
    mTestTexture = new Texture2D(mFont, "Pre rendered test");
}

MenuScene::~MenuScene() {
    delete mFxaaFilter;
    delete mGammaCorrectionFilter;
    delete mGlowFilter;
    delete mGlowBlurFilter;
    delete mPostProcessing;
    
    Resources().FreeFont(mFont);
    
    delete mTestTexture;
}

void MenuScene::Update(float deltaTime) {
    // Updates model matrices for this frame.
    UpdateModelMatrices();
    
    // Render.
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    glViewport(0, 0, screenSize.x, screenSize.y);
    mRenderSystem.Render(*this, mPostProcessing->GetRenderTarget());
    
    // Glow.
    mGlowBlurFilter->SetScreenSize(screenSize);
    int blurAmount = 5;
    for (int i=0; i<blurAmount; ++i) {
        mGlowBlurFilter->SetHorizontal(true);
        mPostProcessing->ApplyFilter(mGlowBlurFilter);
        mGlowBlurFilter->SetHorizontal(false);
        mPostProcessing->ApplyFilter(mGlowBlurFilter);
    }
    mPostProcessing->ApplyFilter(mGlowFilter);
    
    // Anti-aliasing.
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        mFxaaFilter->SetScreenSize(screenSize);
        mPostProcessing->ApplyFilter(mFxaaFilter);
    }
    
    // Gamma correction.
    mGammaCorrectionFilter->SetBrightness((float)GameSettings::GetInstance().GetDouble("Gamma"));
    mPostProcessing->ApplyFilter(mGammaCorrectionFilter);
    
    // Render to back buffer.
    mPostProcessing->Render();
    
    // Test text rendering.
    mFont->RenderText("Test", glm::vec2(0.f, 0.f), 1000.f);
    
    mTestTexture->Render(glm::vec2(0.f, 0.f), screenSize);
}
