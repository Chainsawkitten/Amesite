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
#include "../GameObject/Boss/SpinBoss.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Bullet.hpp"

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
    int height = width = 60;
    int seed = 0;
    int percent = 50;
    int iterations = 10;
    int threshold = 100;

    CaveGenerator::Coordinate playerPosition(width/2, height/2);
    std::vector<CaveGenerator::Coordinate> bossPositions;
    bossPositions.push_back(CaveGenerator::Coordinate(45, 45));

    // Create a map.
    mCave = GameEntityCreator().CreateMap(width, height, seed, percent, iterations, threshold, playerPosition, bossPositions);

    float playerStartX = mCave->scaleFactor*(static_cast<float>(width) / 2.f);
    float playerStartZ = mCave->scaleFactor*(static_cast<float>(height) / 2.f);

    //Stores where the portal is located
    mPortalPosition = glm::vec2(playerStartX, playerStartZ);

    // Create players 
    mPlayers.push_back(GameEntityCreator().CreatePlayer1(glm::vec3(playerStartX+1.f, 0.f, playerStartZ+1.f)));
    mPlayers.push_back(GameEntityCreator().CreatePlayer2(glm::vec3(playerStartX-1.f, 0.f, playerStartZ-1.f)));
    
    // Create boss
    mSpinBoss = GameEntityCreator().CreateSpinBoss(glm::vec3(mCave->scaleFactor*bossPositions[0].x, 0.f, mCave->scaleFactor*bossPositions[0].y));
    
    //Stores how many bosses exist
    mBossCounter = 1;

    mCheckpointSystem.MoveCheckpoint(glm::vec2(playerStartX, playerStartZ));

    // Add players to checkpoint system.
    for (auto& player : mPlayers) {
        mCheckpointSystem.AddPlayer(player);
    }

    // Directional light.
    Entity* dirLight = CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.01f, 0.01f, 0.01f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.24f;
    
    mPostProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    mFxaaFilter = new FXAAFilter();
    mGammaCorrectionFilter = new GammaCorrectionFilter();
    mGlowFilter = new GlowFilter();
    mGlowBlurFilter = new GlowBlurFilter();

    GameEntityCreator().CreateBasicEnemy(glm::vec3(100, 0, 35));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(130, 0, 35));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(150, 0, 55));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(160, 0, 65));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(130, 0, 85));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(110, 0, 55));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(50, 0, 105));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(115, 0, 135));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(175, 0, 135));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(195, 0, 145));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(195, 0, 245));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(225, 0, 235));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(155, 0, 175));
    GameEntityCreator().CreateEnemyPylon(glm::vec3(105, 0, 190));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(55, 0, 190));

    GameEntityCreator().CreateSpawn(glm::vec3(playerStartX + 1.f, -12.f, playerStartZ - 25.f));
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
    // ControllerSystem
    mControllerSystem.Update(*this, deltaTime);

    for (auto player : mPlayers) {
        player->Update();
        mCave->GridCollide(player->GetNodeEntity(), deltaTime);
        //GridCollide(player->GetNodeEntity(), deltaTime, 5);
        if (player->GetHealth() < 0.01f && player->Active()) {
            player->GetNodeEntity()->GetComponent<Component::Physics>()->angularVelocity.y = 2.5f;
            player->Deactivate();
            GameEntityCreator().CreateExplosion(player->GetPosition(), 1.5f, 25.f, Component::ParticleEmitter::BLUE);
        }
        glm::vec2 playerPosition(player->GetPosition().x, player->GetPosition().z);

        if (mBossCounter == 0 && glm::distance(playerPosition, mPortalPosition) < 10.f) {
            Game::GetInstance().SetScene(new WinScene(mTimer, 10));
        }
    }

    // Update boss
    if (mSpinBoss != nullptr)
        mSpinBoss->Update();

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
    
    //std::vector<Component::Damage*> bulletVector = this->GetAll<Component::Damage>();
    //for (auto bullet : bulletVector)
    //    if (GridCollide(bullet->entity, deltaTime, 5.f))
    //        bullet->entity->GetComponent<Component::LifeTime>()->lifeTime = 0.f;
    mGridCollideSystem.Update(*this, deltaTime, *mCave);

    // Update health
    mHealthSystem.Update(*this, deltaTime);

    // Update reflection
    mReflectSystem.Update(*this, deltaTime);
    
    // Update damage
    mDamageSystem.Update(*this);
    
    // Update lifetimes
    mLifeTimeSystem.Update(*this, deltaTime);

    // Update explotion system
    mExplodeSystem.Update(*this);

    // Remove killed entities
    ClearKilledEntities();

    // Update sounds.
    System::SoundSystem::GetInstance()->Update(*this);
    
    // Update game logic
    mMainCamera->UpdateRelativePosition(mPlayers);

    //Handles the respawning of the players
    Respawn(deltaTime);

    mCheckpointSystem.Update();

    if (mSpinBoss != nullptr)
        if (mSpinBoss->GetHealth() < 0.01f) {
            mSpinBoss->Kill();
            mSpinBoss = nullptr;
            mBossCounter--;
        }

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

void MainScene::Respawn(float deltaTime) {

    if (!mPlayers[0]->Active() || !mPlayers[1]->Active())
        if (glm::distance(mPlayers[0]->GetPosition(), mPlayers[1]->GetPosition()) < 15) {

            mPlayers[0]->mRespawnTimer -= deltaTime;
            mPlayers[1]->mRespawnTimer -= deltaTime;

            if (mPlayers[0]->mRespawnTimer <= 0)
                mPlayers[0]->Activate();

            if (mPlayers[1]->mRespawnTimer <= 0)
                mPlayers[1]->Activate();

            mPlayers[0]->GetNodeEntity()->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.3f, 1.f, 0.3f);
            mPlayers[1]->GetNodeEntity()->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.3f, 1.f, 0.3f);

        }
        else {

            mPlayers[0]->mRespawnTimer = 5;
            mPlayers[1]->mRespawnTimer = 5;

            mPlayers[0]->GetNodeEntity()->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.01f, 0.01f, 0.01f);
            mPlayers[1]->GetNodeEntity()->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.01f, 0.01f, 0.01f);

        }
        
}
