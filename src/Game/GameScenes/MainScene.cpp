#include "MainScene.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Util/Input.hpp>
#include "../Util/GameEntityFactory.hpp"
#include "../Util/ControlSchemes.hpp"
#include <Engine/Profiling/CPUProfiling.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/DirectionalLight.hpp>
#include <Engine/Component/SpotLight.hpp>
#include <Engine/Component/Listener.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/SoundSource.hpp>
#include <Engine/Component/Listener.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include "../Component/Health.hpp"
#include "../Component/Damage.hpp"
#include "../Component/LifeTime.hpp"
#include "../Component/Spawner.hpp"
#include <Engine/Component/PointLight.hpp>

#include <Engine/System/SoundSystem.hpp>
#include <Engine/Audio/SoundBuffer.hpp>

#include <Engine/Resources.hpp>
#include <Engine/Texture/Texture2D.hpp>

#include <Engine/PostProcessing/PostProcessing.hpp>
#include <Engine/PostProcessing/FXAAFilter.hpp>
#include <Engine/PostProcessing/GlowFilter.hpp>
#include <Engine/PostProcessing/GlowBlurFilter.hpp>
#include <Engine/PostProcessing/GammaCorrectionFilter.hpp>
#include <Engine/MainWindow.hpp>
#include "../Util/GameSettings.hpp"
#include "../Util/Hub.hpp"
#include "../Util/CaveGenerator.hpp"
#include <Engine/Util/Log.hpp>

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

#include <Engine/RenderTarget.hpp>
#include "../Game.hpp"
#include "WinScene.hpp"

#include <Engine/Geometry/OBJModel.hpp>

using namespace GameObject;

MainScene::MainScene() {
    System::SoundSystem::GetInstance()->SetVolume(static_cast<float>(GameSettings::GetInstance().GetDouble("Audio Volume")));
    
    // Assign input
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::UP, InputHandler::JOYSTICK, InputHandler::D_PAD_UP);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::DOWN, InputHandler::JOYSTICK, InputHandler::D_PAD_DOWN);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::RIGHT, InputHandler::JOYSTICK, InputHandler::D_PAD_RIGHT);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::LEFT, InputHandler::JOYSTICK, InputHandler::D_PAD_LEFT);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::PAUSE, InputHandler::JOYSTICK, InputHandler::START);
    
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::UP, InputHandler::JOYSTICK, InputHandler::D_PAD_UP);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::DOWN, InputHandler::JOYSTICK, InputHandler::D_PAD_DOWN);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::RIGHT, InputHandler::JOYSTICK, InputHandler::D_PAD_RIGHT);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::LEFT, InputHandler::JOYSTICK, InputHandler::D_PAD_LEFT);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::PAUSE, InputHandler::JOYSTICK, InputHandler::START);
    
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::PAUSE, InputHandler::KEYBOARD, GLFW_KEY_ESCAPE);
    
    // Music
    mCalmSoundBuffer = Resources().CreateSound("Resources/sound/MusicCalm.ogg");
    alGenSources(1, &mCalmSource);
    alSourcei(mCalmSource, AL_BUFFER, mCalmSoundBuffer->Buffer());
    alSourcei(mCalmSource, AL_LOOPING, AL_TRUE);
    
    mActionSoundBuffer = Resources().CreateSound("Resources/sound/MusicAction.ogg");
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
    int height = width = 120;
    int seed = time(0);
    int percent = 50;
    int iterations = 10;
    int threshold = 200;
    
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
    // Ensure that killed scenery stays killed.
    ClearKilled();
    
    float playerStartX = mCave->scaleFactor*(static_cast<float>(width) / 2.f);
    float playerStartZ = mCave->scaleFactor*(static_cast<float>(height) / 2.f);
    
    //Stores where the portal is located
    mPortalPosition = glm::vec2(playerStartX, playerStartZ);
    
    // Create players 
    Player1* player1 = GameEntityCreator().CreatePlayer1(glm::vec3(playerStartX + 1.f, 0.f, playerStartZ + 1.f));
    HubInstance().mPlayers.push_back(player1);
    Player2* player2 = GameEntityCreator().CreatePlayer2(glm::vec3(playerStartX + 7.f, 0.f, playerStartZ + 6.f));
    player2->SetYaw(-90);
    HubInstance().mPlayers.push_back(player2);
    
    HubInstance().SetPlayer2State(GameSettings::GetInstance().GetBool("Two Players"));
    
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
            mPillarVector.push_back(GameEntityCreator().CreatePillar(glm::vec3(pillarOrigin.x + i*40.f, pillarOrigin.y, pillarOrigin.z + j*40.f), mBossVector[bossIndex]->GetPosition()));
            mNoSpawnRooms.push_back(glm::vec3(bossPositions[bossIndex].x, 0.f, bossPositions[bossIndex].y));
            bossIndex++;
        }
    }
    
    // Create altar
    GameEntityCreator().CreateAltar(glm::vec3(mPortalPosition.x, -16.f, mPortalPosition.y));
    
    //Stores how many bosses exist
    mBossCounter = mBossVector.size();
    
    mCheckpointSystem.MoveCheckpoint(glm::vec2(playerStartX, playerStartZ));
    
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
    
    mWater.SetTextureRepeat(glm::vec2(100.f, 100.f));
    mWater.SetPosition(glm::vec3(450.f, -4.f, 450.f));
    
    PreallocateTextures();
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
    
    for (Texture2D* texture : mPreallocatedTextures) {
        Resources().FreeTexture2D(texture);
    }
}

void MainScene::Update(float deltaTime) {
    // Pause the game if the player presses the pause button.
    if (!mMenu.IsActive() && Input()->Triggered(InputHandler::ANYONE, InputHandler::PAUSE))
        mMenu.PauseGame();
    
    // We're in the menu, don't update the regular systems.
    if (!mMenu.IsActive()) {
        // Update spawners
        { PROFILE_CPU("Spawner system");
            mSpawnerSystem.Update(*this, deltaTime);
        }
        
        // ControllerSystem
        { PROFILE_CPU("Controller system");
            mControllerSystem.Update(*this, deltaTime);
        }
        
        { PROFILE_CPU("Player collision");
            for (auto player : HubInstance().mPlayers) {
                mCave->GridCollide(player->GetNodeEntity(), deltaTime);
                if (player->GetHealth() < 0.01f && player->Active()) {
                    player->GetNodeEntity()->GetComponent<Component::Physics>()->angularVelocity.y = 2.5f;
                    player->Deactivate();
                    GameEntityCreator().CreateExplosion(player->GetPosition(), 1.f, 10.f, Component::ParticleEmitter::BLUE);
                    GameEntityCreator().CreateReviveCircle(player);
                }
                glm::vec2 playerPosition(player->GetPosition().x, player->GetPosition().z);
                
                if (mBossCounter == 0 && glm::distance(playerPosition, mPortalPosition) < 2.f)
                    Game::GetInstance().SetScene(new WinScene(mTimer, mEnemySpawnerSystem.GetEnemiesKilled(), mCheckpointSystem.timesDied));
            }
        }
        
        // AnimationSystem.
        { PROFILE_CPU("Animation system");
            mAnimationSystem.Update(*this, deltaTime);
        }
        
        // PhysicsSystem.
        { PROFILE_CPU("Physics system");
            mPhysicsSystem.Update(*this, deltaTime);
        }
    }
    
    // Updates model matrices for this frame.
    { PROFILE_CPU("Update model matrices");
        UpdateModelMatrices();
    }
    
    // ParticleSystem
    { PROFILE_CPU("Particle system");
        System::Particle().Update(*this, deltaTime);
    }
    
    if (!mMenu.IsActive()) {
        // Check collisions.
        { PROFILE_CPU("Collision system");
            mCollisionSystem.Update(*this);
        }
        
        // Update enemy spawning
        { PROFILE_CPU("Enemy spawner system");
            mEnemySpawnerSystem.Update(*this, deltaTime, mCave, mNoSpawnRooms);
        }
        
        // Check grid collisions.
        { PROFILE_CPU("Grid collide system");
            mGridCollideSystem.Update(*this, deltaTime, *mCave);
        }
        
        // Update health
        { PROFILE_CPU("Health system");
            mHealthSystem.Update(*this, deltaTime);
        }
        
        // Update reflection
        { PROFILE_CPU("Reflect system");
            mReflectSystem.Update(*this, deltaTime);
        }
        
        // Update damage
        { PROFILE_CPU("Damage system");
            mDamageSystem.Update(*this);
        }
        
        // Update lifetimes
        { PROFILE_CPU("Lifetime system");
            mLifeTimeSystem.Update(*this, deltaTime);
        }
        
        // UpdateSystem.
        { PROFILE_CPU("Update system");
            mUpdateSystem.Update(*this, deltaTime);
        }
    }
    
    // Update sounds.
    { PROFILE_CPU("Sound system");
        System::SoundSystem::GetInstance()->Update(*this);
    }
    
    // Update camera.
    Component::Transform* cameraTransform = mMainCamera->body->GetComponent<Component::Transform>();
    cameraTransform->yaw = 0.f;
    cameraTransform->pitch = 60.f;
    cameraTransform->roll = 0.f;
    
    mMainCamera->UpdateRelativePosition(mBossVector, deltaTime);
    
    if (!mMenu.IsActive()) {
        // If all players are disabled, respawn them.
        { PROFILE_CPU("Checkpoint system");
            mCheckpointSystem.Update(deltaTime);
        }
        
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
        { PROFILE_CPU("Explosion system");
            mExplodeSystem.Update(*this);
        }
        
        // Remove killed entities
        { PROFILE_CPU("Clear killed");
            ClearKilled();
        }
    }
    
    if (mMenu.IsActive())
        mMenu.Update(HubInstance().mPlayers[0], deltaTime);
    
    // Water.
    mWater.Update(deltaTime, glm::vec3(4.f, 0.f, 1.f));
    
    { PROFILE_CPU("Update particle buffers");
        mParticleRenderSystem.UpdateBuffer(*this);
    }
    
    const glm::vec2& screenSize = MainWindow::GetInstance()->GetSize();
    
    // Render refractions.
    { PROFILE_CPU("Render refractions");
        if (GameSettings::GetInstance().GetBool("Refractions")) {
            mRenderSystem.Render(*this, mWater.GetRefractionTarget(), mWater.GetRefractionTarget()->GetSize(), mWater.GetRefractionClippingPlane());
            // Don't render particle refractions as they're never below the water level.
            //mParticleRenderSystem.Render(*this, mMainCamera->body, mWater.GetRefractionTarget()->GetSize(), mWater.GetRefractionClippingPlane());
        } else {
            mWater.GetRefractionTarget()->SetTarget();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }
    }
    
    // Render reflections
    { PROFILE_CPU("Render reflections");
        if (GameSettings::GetInstance().GetBool("Reflections")) {
            /// @todo Don't hardcore camera inversion.
            float distance = 2.f * (cameraTransform->position.y - mWater.GetPosition().y);
            cameraTransform->position = cameraTransform->position - glm::vec3(0.f, distance, 0.f);
            cameraTransform->pitch = -cameraTransform->pitch;
            cameraTransform->UpdateModelMatrix();
            mRenderSystem.Render(*this, mWater.GetReflectionTarget(), mWater.GetReflectionTarget()->GetSize(), mWater.GetReflectionClippingPlane());
            mParticleRenderSystem.Render(*this, mMainCamera->body, mWater.GetReflectionTarget()->GetSize(), mWater.GetReflectionClippingPlane());
            cameraTransform->pitch = -cameraTransform->pitch;
            cameraTransform->position = cameraTransform->position + glm::vec3(0.f, distance, 0.f);
            cameraTransform->UpdateModelMatrix();
        } else {
            mWater.GetReflectionTarget()->SetTarget();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }
    }
    
    // Render.
    { PROFILE_CPU("Render system");
        mRenderSystem.Render(*this, mPostProcessing->GetRenderTarget(), screenSize);
    }
    
    if (GameSettings::GetInstance().GetBool("Refractions") || GameSettings::GetInstance().GetBool("Reflections"))
        mWater.Render();
    
    if (mMenu.IsActive())
        mMenu.RenderSelected();
    
    { PROFILE_CPU("Anti-aliasing");
        // Anti-aliasing.
        if (GameSettings::GetInstance().GetBool("FXAA")) {
            mFxaaFilter->SetScreenSize(screenSize);
            mFxaaFilter->SetBrightness((float)GameSettings::GetInstance().GetDouble("Gamma"));
            mPostProcessing->ApplyFilter(mFxaaFilter);
        }
    }
    
    { PROFILE_CPU("Render particles");
        mParticleRenderSystem.Render(*this, mMainCamera->body, screenSize);
    }
    
    // Glow.
    { PROFILE_CPU("Glow");
        mGlowBlurFilter->SetScreenSize(screenSize);
        int blurAmount = 1;
        for (int i = 0; i < blurAmount; ++i) {
            mGlowBlurFilter->SetHorizontal(true);
            mPostProcessing->ApplyFilter(mGlowBlurFilter);
            mGlowBlurFilter->SetHorizontal(false);
            mPostProcessing->ApplyFilter(mGlowBlurFilter);
        }
        mPostProcessing->ApplyFilter(mGlowFilter);
    }
    
    // Gamma correction.
    { PROFILE_CPU("Gamma correction");
        mGammaCorrectionFilter->SetBrightness((float)GameSettings::GetInstance().GetDouble("Gamma"));
        mPostProcessing->ApplyFilter(mGammaCorrectionFilter);
    }
    
    // Render to back buffer.
    { PROFILE_CPU("Render to back buffer");
        mPostProcessing->Render(GameSettings::GetInstance().GetBool("Dithering"));
    }
    
    if (mMenu.IsActive())
        mMenu.RenderMenuOptions();
    
    mTimer += deltaTime;
    
    // Set music volumes.
    mTargetMix = 0.f;
    for (GameObject::SuperEnemy* enemy : mEnemySpawnerSystem.GetEnemies()) {
        if (mCheckpointSystem.mRespawn) {
            if (glm::distance(enemy->node->GetComponent<Component::Transform>()->GetWorldPosition(), HubInstance().mPlayers[0]->GetPosition()) < 10) {
                enemy->node->GetComponent<Component::Health>()->health = 0;
            }
        }
        
        if (enemy->Active()) {
            mTargetMix = 1.f;
            break;
        }
    }
    
    mCheckpointSystem.mRespawn = false;
    
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

void MainScene::PreallocateTextures() {
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/wall_gray.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/enemy_spec.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/enemy_diff.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/enemy_spec.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/enemy_glow.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/DefaultBlue.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/enemy_spec.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/default/DefaultSpecular.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/spawn/pillar/Pillar_Albedo.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/spawn/pillar/Pillar_NM.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/spawn/pillar/Pillar_Glow2.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/color/DefaultGray.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/stones/stone_02_glow.png"));
    mPreallocatedTextures.push_back(Resources().CreateTexture2DFromFile("Resources/stones/stone_02_diff.png"));
}
