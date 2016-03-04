#include "MainScene.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Util/Input.hpp>
#include "Game/Util/GameEntityFactory.hpp"
#include "Game/Util/ControlSchemes.hpp"

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/DirectionalLight.hpp>
#include <Component/SpotLight.hpp>
#include <Component/Listener.hpp>
#include <Component/Physics.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/SoundSource.hpp>
#include <Component/Listener.hpp>
#include <Component/ParticleEmitter.hpp>
#include "Game/Component/Health.hpp"
#include "Game/Component/Damage.hpp"
#include "Game/Component/LifeTime.hpp"
#include "Game/Component/Spawner.hpp"

#include <System/SoundSystem.hpp>
#include <Audio/SoundBuffer.hpp>

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>

#include <PostProcessing/PostProcessing.hpp>
#include <PostProcessing/FXAAFilter.hpp>
#include <PostProcessing/GlowFilter.hpp>
#include <PostProcessing/GlowBlurFilter.hpp>
#include <PostProcessing/GammaCorrectionFilter.hpp>
#include <MainWindow.hpp>
#include "../Util/GameSettings.hpp"
#include "../Util/MainCamera.hpp"
#include "../Util/CaveGenerator.hpp"
#include <Util/Log.hpp>

#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"
#include "../GameObject/Boss/SuperBoss.hpp"
#include "../GameObject/Boss/SpinBoss.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Bullet.hpp"
#include "../GameObject/Altar.hpp"
#include "../GameObject/Pillar.hpp"


#include "../Game.hpp"
#include "WinScene.hpp"

using namespace GameObject;

MainScene::MainScene() {
    System::SoundSystem::GetInstance()->SetVolume(static_cast<float>(GameSettings::GetInstance().GetDouble("Audio Volume")));
    
    // Music
    mMusicSoundBuffer = Resources().CreateSound("Resources/MusicCalm.ogg");
    alGenSources(1, &mSource);
    alSourcei(mSource, AL_BUFFER, mMusicSoundBuffer->Buffer());
    alSourcei(mSource, AL_LOOPING, AL_TRUE);
    alSourcePlay(mSource);
    
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);

    // Set timer to 0
    mTimer = 0.f;
    
    // Create main camera
    mMainCamera = GameEntityCreator().CreateCamera(glm::vec3(300.f, 300.f, 300.f), glm::vec3(0.f, 60.f, 0.f));
    MainCameraInstance().SetMainCamera(mMainCamera->body);

    // Create scene
    int width;
    int height = width = 90;
    int seed = 0;
    int percent = 50;
    int iterations = 10;
    int threshold = 100;

    CaveGenerator::Coordinate playerPosition(width/2, height/2);
    CaveGenerator::Coordinate NorthWest(5, 5);
    CaveGenerator::Coordinate SouthEast(height-5, width-5);
    CaveGenerator::Coordinate NorthEast(height-5, 5);
    CaveGenerator::Coordinate SouthWest(5, width - 5);

    std::vector<CaveGenerator::Coordinate> bossPositions;
    bossPositions.push_back(NorthWest);
    bossPositions.push_back(SouthWest);
    bossPositions.push_back(SouthEast);
    bossPositions.push_back(NorthEast);

    // Create a map.
    mCave = GameEntityCreator().CreateMap(width, height, seed, percent, iterations, threshold, playerPosition, bossPositions);

    float playerStartX = mCave->scaleFactor*(static_cast<float>(width) / 2.f);
    float playerStartZ = mCave->scaleFactor*(static_cast<float>(height) / 2.f);

    //Stores where the portal is located
    mPortalPosition = glm::vec2(playerStartX, playerStartZ);

    // Create players 
    mPlayers.push_back(GameEntityCreator().CreatePlayer1(glm::vec3(playerStartX+1.f, 0.f, playerStartZ+1.f)));
    mPlayers.push_back(GameEntityCreator().CreatePlayer2(glm::vec3(playerStartX-1.f, 0.f, playerStartZ-1.f)));
    
    // Create bosses and pillars
    for (int i = 0; i < bossPositions.size(); i++) {
        mBossVector.push_back(GameEntityCreator().CreateSpinBoss(glm::vec3(mCave->scaleFactor*bossPositions[i].x, 0.f, mCave->scaleFactor*bossPositions[i].y)));
        mPillarVector.push_back(GameEntityCreator().CreatePillar(glm::vec3(mPortalPosition.x - 15.f + 15.f * i, -8.f, playerStartZ + 25.f - 2.f * i), mBossVector[i]->GetPosition()));
        mNoSpawnRooms.push_back(glm::vec3(bossPositions[i].x, 0.f, bossPositions[i].y));
    }

    // Create altar
    GameEntityCreator().CreateAltar(glm::vec3(mPortalPosition.x, -16.f, mPortalPosition.y));
        
    //Stores how many bosses exist
    mBossCounter = mBossVector.size();

    mCheckpointSystem.MoveCheckpoint(glm::vec2(playerStartX, playerStartZ));

    // Add players to checkpoint system.
    for (auto& player : mPlayers) {
        mCheckpointSystem.AddPlayer(player);
    }

    // Directional light.
    Entity* dirLight = CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.0000001f, 0.0000001f, 0.0000001f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.005f;
    
    mPostProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    mFxaaFilter = new FXAAFilter();
    mGammaCorrectionFilter = new GammaCorrectionFilter();
    mGlowFilter = new GlowFilter();
    mGlowBlurFilter = new GlowBlurFilter();

    GameEntityCreator().CreateEnemySpawner(Component::Spawner::PYLON, 2);
    GameEntityCreator().CreateEnemySpawner(Component::Spawner::BASIC, 5);

    // Push boss positions here to avoid spawning enemies.
    mNoSpawnRooms.push_back(glm::vec3(playerStartX / mCave->scaleFactor, 0.f, playerStartZ / mCave->scaleFactor));

}

MainScene::~MainScene() {
    delete mFxaaFilter;
    delete mGammaCorrectionFilter;
    delete mGlowFilter;
    delete mGlowBlurFilter;
    delete mPostProcessing;
    
    alDeleteSources(1, &mSource);
    Resources().FreeSound(mMusicSoundBuffer);
}

void MainScene::Update(float deltaTime) {
    // Update spawners
    mSpawnerSystem.Update(*this, deltaTime);

    // ControllerSystem
    mControllerSystem.Update(*this, deltaTime);

    for (auto player : mPlayers) {
        mCave->GridCollide(player->GetNodeEntity(), deltaTime);
        if (player->GetHealth() < 0.01f && player->Active()) {
            player->GetNodeEntity()->GetComponent<Component::Physics>()->angularVelocity.y = 2.5f;
            player->Deactivate();
            GameEntityCreator().CreateExplosion(player->GetPosition(), 1.5f, 25.f, Component::ParticleEmitter::BLUE);
        }
        glm::vec2 playerPosition(player->GetPosition().x, player->GetPosition().z);

        if (mBossCounter == 0 && glm::distance(playerPosition, mPortalPosition) < 2.f)
            Game::GetInstance().SetScene(new WinScene(mTimer, mEnemySpawnerSystem.GetEnemiesKilled()));
    }

    // AnimationSystem.
    mAnimationSystem.Update(*this, deltaTime);

    // PhysicsSystem.
    mPhysicsSystem.Update(*this, deltaTime);
    
    // Updates model matrices for this frame.
    UpdateModelMatrices();

    // ParticleSystem
    System::Particle().Update(*this, deltaTime);
    
    // Check collisions.
    mCollisionSystem.Update(*this);

    // Update enemy spawning
    mEnemySpawnerSystem.Update(*this, deltaTime, mCave, &mPlayers, mNoSpawnRooms);
    
    // Check grid collisions.
    mGridCollideSystem.Update(*this, deltaTime, *mCave);

    // Update health
    mHealthSystem.Update(*this, deltaTime);

    // Update reflection
    mReflectSystem.Update(*this, deltaTime);
    
    // Update damage
    mDamageSystem.Update(*this);
    
    // Update lifetimes
    mLifeTimeSystem.Update(*this, deltaTime);
    
    // UpdateSystem.
    mUpdateSystem.Update(*this, deltaTime);
    
    // Update sounds.
    System::SoundSystem::GetInstance()->Update(*this);
    
    // Update game logic
    mMainCamera->UpdateRelativePosition(mPlayers);

    //If all players are disabled, respawn them.
    mCheckpointSystem.Update(deltaTime);

    for (int i = 0; i < mBossVector.size(); i++) {
        if (mBossVector[i] != nullptr)
            if (mBossVector[i]->GetHealth() < 0.01f) {
                mBossVector[i]->Kill();
                mBossVector[i] = nullptr;
                mBossCounter--;
                mPillarVector[i]->SetState(GameObject::Pillar::ACTIVE);
                if (mBossCounter == 0)
                    GameEntityCreator().CreatePortal(glm::vec3(mPortalPosition.x, 0.f, mPortalPosition.y));
            }
    }
    
    // Update explosion system
    mExplodeSystem.Update(*this);
    
    // Remove killed entities
    ClearKilled();

    // Render.
    mRenderSystem.Render(*this, mPostProcessing->GetRenderTarget());
    
    // Glow.
    mGlowBlurFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
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
        mFxaaFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
        mPostProcessing->ApplyFilter(mFxaaFilter);
    }
    
    // Gamma correction.
    mGammaCorrectionFilter->SetBrightness((float)GameSettings::GetInstance().GetDouble("Gamma"));
    mPostProcessing->ApplyFilter(mGammaCorrectionFilter);
    
    // Render to back buffer.
    mPostProcessing->Render();

    mTimer += deltaTime;

}
