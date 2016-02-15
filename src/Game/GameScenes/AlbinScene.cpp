#include "AlbinScene.hpp"

#include <Resources.hpp>
#include <Audio/SoundBuffer.hpp>
#include <Audio/Listener.hpp>

#include "../Util/GameSettings.hpp"

#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Physics.hpp>
#include <Component/SoundSource.hpp>

#include <MainWindow.hpp>
#include <PostProcessing/PostProcessing.hpp>
#include <PostProcessing/FXAAFilter.hpp>
#include <PostProcessing/GammaCorrectionFilter.hpp>

AlbinScene::AlbinScene() {
    mSoundSystem.GetListener()->SetGain(GameSettings::GetInstance().GetDouble("Audio Volume"));
    
    mTestSoundBuffer = Resources().CreateSound("Resources/Laser.ogg");
    
    s1 = CreateEntity();
    Component::Transform* transform = s1->AddComponent<Component::Transform>();
    transform->position = glm::vec3(-10.f, 0.f, 0.f);
    s1->AddComponent<Component::Physics>()->velocity = glm::vec3(1.f, 0.f, 0.f);
    Component::SoundSource* ss1 = s1->AddComponent<Component::SoundSource>();
    ss1->soundBuffer = mTestSoundBuffer;
    ss1->loop = AL_TRUE;
    ss1->Play();
    
    postProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    fxaaFilter = new FXAAFilter();
    gammaCorrectionFilter = new GammaCorrectionFilter();
}

AlbinScene::~AlbinScene() {
    RemoveEntity(s1);
    
    Resources().FreeSound(mTestSoundBuffer);
    
    delete fxaaFilter;
    delete gammaCorrectionFilter;
    delete postProcessing;
}

void AlbinScene::Update(float deltaTime) {
    mPhysicsSystem.Update(*this, deltaTime);
    
    // Updates model matrices for this frame.
    UpdateModelMatrices();
    
    mSoundSystem.Update(*this);
    
    // Render.
    mRenderSystem.Render(*this, postProcessing->GetRenderTarget());
    
    // Apply post-processing effects.
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        fxaaFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
        postProcessing->ApplyFilter(fxaaFilter);
    }
    
    gammaCorrectionFilter->SetBrightness(GameSettings::GetInstance().GetDouble("Gamma"));
    postProcessing->ApplyFilter(gammaCorrectionFilter);
    
    postProcessing->Render();
}
