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
#include "../Util/Hub.hpp"
#include "../Util/CaveGenerator.hpp"
#include <Util/Log.hpp>

#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"
#include "../GameObject/Boss/SuperBoss.hpp"
#include "../GameObject/Boss/SpinBoss.hpp"
#include "../GameObject/Boss/ShieldBoss.hpp"
#include "../GameObject/Boss/DivideBoss.hpp"
#include "../GameObject/Boss/RingBoss.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Bullet.hpp"
#include "../GameObject/Altar.hpp"
#include "../GameObject/Pillar.hpp"
#include "../GameObject/Enemy/SuperEnemy.hpp"


#include "../Game.hpp"
#include "WinScene.hpp"

using namespace GameObject;

MainScene::MainScene() {
    System::SoundSystem::GetInstance()->SetVolume(static_cast<float>(GameSettings::GetInstance().GetDouble("Audio Volume")));

    // Assign input
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);

    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);

    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);

    // Music
    mCalmSoundBuffer = Resources().CreateSound("Resources/MusicCalm.ogg");
    alGenSources(1, &mCalmSource);
    alSourcei(mCalmSource, AL_BUFFER, mCalmSoundBuffer->Buffer());
    alSourcei(mCalmSource, AL_LOOPING, AL_TRUE);

    mActionSoundBuffer = Resources().CreateSound("Resources/MusicAction.ogg");
    alGenSources(1, &mActionSource);
    alSourcef(mActionSource, AL_GAIN, 0.f);
    alSourcei(mActionSource, AL_BUFFER, mActionSoundBuffer->Buffer());
    alSourcei(mActionSource, AL_LOOPING, AL_TRUE);

    alSourcePlay(mCalmSource);
    alSourcePlay(mActionSource);

    mMix = 0.f;
    mTargetMix = 0.f;

    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);

    // Set timer to 0
    mTimer = 0.f;

    // Create main camera
    mMainCamera = GameEntityCreator().CreateCamera(glm::vec3(300.f, 300.f, 300.f), glm::vec3(0.f, 60.f, 0.f));
    HubInstance().SetMainCamera(mMainCamera);

    // Create scene
    int width;
    int height = width = 90;
    int seed = time(0);
    int percent = 50;
    int iterations = 10;
    int threshold = 100;

    CaveGenerator::Coordinate playerPosition(width / 2, height / 2);
    CaveGenerator::Coordinate NorthWest(10, 10);
    CaveGenerator::Coordinate SouthEast(height - 10, width - 10);
    CaveGenerator::Coordinate NorthEast(height - 10, 10);
    CaveGenerator::Coordinate SouthWest(10, width - 10);

    std::vector<CaveGenerator::Coordinate> bossPositions;
    bossPositions.push_back(NorthWest);
    bossPositions.push_back(SouthWest);
    bossPositions.push_back(NorthEast);
    bossPositions.push_back(SouthEast);

    // Create a map.
    mCave = GameEntityCreator().CreateMap(width, height, seed, percent, iterations, threshold, playerPosition, bossPositions);

    float playerStartX = mCave->scaleFactor*(static_cast<float>(width) / 2.f);
    float playerStartZ = mCave->scaleFactor*(static_cast<float>(height) / 2.f);

    //Stores where the portal is located
    mPortalPosition = glm::vec2(playerStartX, playerStartZ);

    // Create players 
    Player1* player1 = GameEntityCreator().CreatePlayer1(glm::vec3(playerStartX + 1.f, 0.f, playerStartZ + 1.f));
    Player2* player2 = GameEntityCreator().CreatePlayer2(glm::vec3(playerStartX - 1.f, 0.f, playerStartZ - 1.f));
    mPlayers.push_back(player1);
    mPlayers.push_back(player2);
    HubInstance().mPlayers.push_back(player1);
    HubInstance().mPlayers.push_back(player2);

    // Set menu position.
    mMenu.SetPosition(glm::vec3(0.f, 4.f, 11.5f));
    mMenu.SetRotation(glm::vec3(0.f, 330.f, 0.f));

    // Create bosses and pillars
    mBossVector.push_back(GameEntityCreator().CreateSpinBoss(glm::vec3(mCave->scaleFactor*bossPositions[0].x, 0.f, mCave->scaleFactor*bossPositions[0].y)));
    mBossVector.push_back(GameEntityCreator().CreateShieldBoss(glm::vec3(mCave->scaleFactor*bossPositions[1].x, 0.f, mCave->scaleFactor*bossPositions[1].y)));
    mBossVector.push_back(GameEntityCreator().CreateRingBoss(glm::vec3(mCave->scaleFactor*bossPositions[2].x, 0.f, mCave->scaleFactor*bossPositions[2].y)));
    mBossVector.push_back(GameEntityCreator().CreateDivideBoss(glm::vec3(mCave->scaleFactor*bossPositions[3].x, 0.f, mCave->scaleFactor*bossPositions[3].y)));
    int numberOfBossPositions = bossPositions.size();

    glm::vec3 pillarOrigin(mPortalPosition.x - 15.f, -0.f, mPortalPosition.y - 15.f);
    int bossIndex = 0;
    for (int i = 0; i < (numberOfBossPositions / 2); i++) {
        for (int j = 0; j < (numberOfBossPositions / 2); j++) {
            mPillarVector.push_back(GameEntityCreator().CreatePillar(glm::vec3(pillarOrigin.x + i*30.f, pillarOrigin.y, pillarOrigin.z + j*30.f), mBossVector[bossIndex]->GetPosition()));
            mNoSpawnRooms.push_back(glm::vec3(bossPositions[bossIndex].x, 0.f, bossPositions[bossIndex].y));
            bossIndex++;
        }
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
    GameEntityCreator().CreateEnemySpawner(Component::Spawner::ROCKET, 5);
    GameEntityCreator().CreateEnemySpawner(Component::Spawner::NEST, 2);

    // Push boss positions here to avoid spawning enemies.
    mNoSpawnRooms.push_back(glm::vec3(playerStartX / mCave->scaleFactor, 0.f, playerStartZ / mCave->scaleFactor));
}

MainScene::~MainScene() {
    delete mFxaaFilter;
    delete mGammaCorrectionFilter;
    delete mGlowFilter;
    delete mGlowBlurFilter;
    delete mPostProcessing;

    alDeleteSources(1, &mCalmSource);
    Resources().FreeSound(mCalmSoundBuffer);

    alDeleteSources(1, &mActionSource);
    Resources().FreeSound(mActionSoundBuffer);
}

void MainScene::Update(float deltaTime) {
    // We're in the menu, don't update the regular systems.
    if (!mMenu.IsActive()) {
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
    }

    // Updates model matrices for this frame.
    UpdateModelMatrices();

    // ParticleSystem
    System::Particle().Update(*this, deltaTime);

    if (!mMenu.IsActive()) {
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
    }

    // Update sounds.
    System::SoundSystem::GetInstance()->Update(*this);

    // Update camera.
    Component::Transform* cameraTransform = mMainCamera->body->GetComponent<Component::Transform>();
    cameraTransform->yaw = 0.f;
    cameraTransform->pitch = 60.f;
    cameraTransform->roll = 0.f;
    mMainCamera->UpdateRelativePosition(mPlayers);

    if (!mMenu.IsActive()) {
        //If all players are disabled, respawn them.
        mCheckpointSystem.Update(deltaTime);

        int bossVectorSize = mBossVector.size();
        for (int i = 0; i < bossVectorSize; i++) {
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
    }

    if (mMenu.IsActive())
        mMenu.Update(mPlayers[0], deltaTime);

    // Render.
    mRenderSystem.Render(*this, mPostProcessing->GetRenderTarget());

    if (mMenu.IsActive())
        mMenu.RenderSelected();

    // Glow.
    mGlowBlurFilter->SetScreenSize(MainWindow::GetInstance()->GetSize());
    int blurAmount = 1;
    for (int i = 0; i < blurAmount; ++i) {
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

    if (mMenu.IsActive())
        mMenu.RenderMenuOptions();

    mTimer += deltaTime;

    // Set music volumes.
    mTargetMix = 0.f;
    for (GameObject::SuperEnemy* enemy : mEnemySpawnerSystem.GetEnemies()) {
        if (enemy->Active()) {
            mTargetMix = 1.f;
            break;
        }
    }

    for (GameObject::SuperBoss* boss : mBossVector) {
        if (boss != nullptr) {
            if (boss->Active()) {
                mTargetMix = 1.f;
                break;
            }
        }
    }

    if (mTargetMix > mMix)
        mMix += deltaTime;
    else if (mTargetMix < mMix)
        mMix -= deltaTime;

    mMix = mMix < 0.f ? 0.f : (mMix > 1.f ? 1.f : mMix);

    alSourcef(mCalmSource, AL_GAIN, 1.f - mMix);
    alSourcef(mActionSource, AL_GAIN, mMix);
}
