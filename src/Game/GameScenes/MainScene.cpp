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

#include "../GameObject/Player.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Camera.hpp"

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

#include "../GameObject/Player.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/SpinBoss.hpp"
#include "../GameObject/Bullet.hpp"

using namespace GameObject;

MainScene::MainScene() {
    System::SoundSystem::GetInstance()->SetVolume(GameSettings::GetInstance().GetDouble("Audio Volume"));
    
    // Assign input
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHIELD, InputHandler::JOYSTICK, InputHandler::LEFT_BUMPER);

    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);
    
    // Music
    mMusicSoundBuffer = Resources().CreateSound("Resources/MusicCalm.ogg");
    alGenSources(1, &mSource);
    alSourcei(mSource, AL_BUFFER, mMusicSoundBuffer->Buffer());
    alSourcei(mSource, AL_LOOPING, AL_TRUE);
    alSourcePlay(mSource);
    
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);
    
    // Create main camera
    mMainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 70.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    MainCameraInstance().SetMainCamera(mMainCamera->body);
    
    // Create scene
    int width = 60;
    int height = 60;
    int seed = 0;
    int percent = 50;
    int iterations = 10;
    int threshold = 40;

    CaveGenerator::Coordinate playerPosition(width/2, height/2);
    std::vector<CaveGenerator::Coordinate> bossPositions;
    bossPositions.push_back(CaveGenerator::Coordinate(45, 45));

    // Create a map.
    mCave = GameEntityCreator().CreateMap(width, height, seed, percent, iterations, threshold, playerPosition, bossPositions);

    float playerStartX = mCave->xScale*(static_cast<float>(width) / 2.f);
    float playerStartZ = mCave->zScale*(static_cast<float>(height) / 2.f);

    // Create players 
    mPlayers.push_back(GameEntityCreator().CreatePlayer(glm::vec3(playerStartX+1.f, 0.f, playerStartZ+1.f), InputHandler::PLAYER_ONE));
    mPlayers.push_back(GameEntityCreator().CreatePlayer(glm::vec3(playerStartX-1.f, 0.f, playerStartZ-1.f), InputHandler::PLAYER_TWO));
    
    // Create boss
    mBosses.push_back(GameEntityCreator().CreateSpinBoss(glm::vec3(mCave->xScale*bossPositions[0].x, 0.f, mCave->zScale*bossPositions[0].y)));

    // Directional light.
    Entity* dirLight = CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.01f, 0.01f, 0.01f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.01f;
    
    postProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    fxaaFilter = new FXAAFilter();
    gammaCorrectionFilter = new GammaCorrectionFilter();
    glowFilter = new GlowFilter();
    glowBlurFilter = new GlowBlurFilter();

    GameEntityCreator().CreateEnemyPylon(glm::vec3(80, 0, 25));
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
}

MainScene::~MainScene() {
    delete fxaaFilter;
    delete gammaCorrectionFilter;
    delete glowFilter;
    delete glowBlurFilter;
    delete postProcessing;
    
    alDeleteSources(1, &mSource);
    Resources().FreeSound(mMusicSoundBuffer);
}

void MainScene::Update(float deltaTime) {
    // ControllerSystem
    mControllerSystem.Update(*this, deltaTime);

    for (auto player : mPlayers) {
        GridCollide(player->node, deltaTime, 5);
        if (player->GetHealth() < 0.01f) {
            player->node->GetComponent<Component::Physics>()->angularVelocity.y = 2.5f;
            player->node->GetComponent<Component::Health>()->health = player->node->GetComponent<Component::Health>()->maxHealth;
            GameEntityCreator().CreateExplosion(player->GetPosition(), 1.5f, 25.f, Component::ParticleEmitter::BLUE);
        }
    }

    for (auto boss : mBosses)
        boss->Update();


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
    
    std::vector<Component::Damage*> bulletVector = this->GetAll<Component::Damage>();
    for (auto bullet : bulletVector)
        if (GridCollide(bullet->entity, deltaTime, 5.f))
            bullet->entity->GetComponent<Component::LifeTime>()->lifeTime = 0.f;

    // Update health
    mHealthSystem.Update(*this, deltaTime);

    // Update reflection
    mReflectSystem.Update(*this, deltaTime);
    
    // Update damage
    mDamageSystem.Update(*this);
    
    // Update lifetimes
    mLifeTimeSystem.Update(*this, deltaTime);

    // Remove killed game objects
    ClearKilledGameObjects();

    // Update sounds.
    System::SoundSystem::GetInstance()->Update(*this);
    
    // Update game logic
    mMainCamera->UpdateRelativePosition(mPlayers);

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
}

int PointCollide(glm::vec3 point, glm::vec3 velocity, float deltaTime, float gridScale) {
    int oldX = glm::floor(point.x / gridScale);
    int oldZ = glm::floor(point.z / gridScale);
    int newX = glm::floor((point + velocity * deltaTime).x / gridScale);
    int newZ = glm::floor((point + velocity * deltaTime).z / gridScale);

    float X = (newX - oldX) / velocity.x;
    float Z = (newZ - oldZ) / velocity.z;

    //We check if we moved to another cell in the grid.
    if (GameObject::Cave::mMap[abs(newZ)][abs(newX)]) {
        //We collide in X
        if (X > Z) {

            if (oldX != newX) {
                return 0;
            } else if (oldZ != newZ) {
                return 1;
            }
        }
        //We collide in Z
        else {
            if (oldZ != newZ) {
                return 1;
            } else if (oldX != newX) {
                return 0;
            }
        }
    }
    return -1;
}

bool MainScene::GridCollide(Entity* entity, float deltaTime, float gridScale) {

    Component::Transform* transform = entity->GetComponent<Component::Transform>();
    Component::Physics* physics = entity->GetComponent<Component::Physics>();

    glm::vec3 velocity = physics->velocity;
    velocity += physics->acceleration * deltaTime;
    velocity -= physics->velocity * physics->velocityDragFactor * deltaTime;

    glm::vec3 width = glm::vec3(transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f, 0, 0);
    glm::vec3 height = glm::vec3(0, 0, transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f);

    //glm::vec3 width = glm::vec3(2.9f, 0.f, 0.f);
    //glm::vec3 height = glm::vec3(0.f, 0.f, 2.9f);

    int c0 = PointCollide(transform->CalculateWorldPosition() - width - height, velocity, deltaTime, gridScale);
    int c1 = PointCollide(transform->CalculateWorldPosition() + width - height, velocity, deltaTime, gridScale);
    int c2 = PointCollide(transform->CalculateWorldPosition() + width + height, velocity, deltaTime, gridScale);
    int c3 = PointCollide(transform->CalculateWorldPosition() - width + height, velocity, deltaTime, gridScale);

    switch (c0) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }
    switch (c1) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }
    switch (c2) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }
    switch (c3) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }

    if (c0 != -1 || c1 != -1 || c2 != -1 || c3 != -1)
        return true;

    return false;

}
