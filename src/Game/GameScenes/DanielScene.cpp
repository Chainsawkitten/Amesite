#include "DanielScene.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Util/Input.hpp>
#include "Game/Util/CameraUpdate.hpp"
#include "Game/Util/GameEntityFactory.hpp"
#include "Game/Util/ControlSchemes.hpp"

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/DirectionalLight.hpp>
#include <Component/SpotLight.hpp>
#include <Component/Physics.hpp>
#include <Component/Collider2DCircle.hpp>
#include "../GameObject/Player.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Camera.hpp"
#include <Component/ParticleEmitter.hpp>
#include "Game/Component/Health.hpp"
#include <Component/ParticleEmitter.hpp>

#include <Texture/Texture2D.hpp>

#include <PostProcessing/PostProcessing.hpp>
#include <PostProcessing/FXAAFilter.hpp>
#include <PostProcessing/GammaCorrectionFilter.hpp>
#include <MainWindow.hpp>
#include "../Util/GameSettings.hpp"
#include "../Util/MainCamera.hpp"
#include <Util/Picking.hpp>
#include <Util/Input.hpp>
#include <Util/Log.hpp>

using namespace GameObject;

DanielScene::DanielScene() {
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
    
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);
    
    // Bind scene to gameEntityCreator
    GameEntityCreator().SetScene(this);
    
    mParticleSystem.SetActive();
    
    // Create main camera
    Camera* mainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    mMainCamera = mainCamera->GetEntity("body");
    MainCameraInstance().setMainCamera(mMainCamera);
    
    // Create players
    Player* player1 = GameEntityCreator().CreatePlayer(glm::vec3(-4.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    Player* player2 = GameEntityCreator().CreatePlayer(glm::vec3(1.f, 0.f, 1.f), InputHandler::PLAYER_TWO);
    
    mPlayers.push_back(player1->GetEntity("body"));
    mPlayers.push_back(player2->GetEntity("body"));
    
    // Create scene
    cave = GameEntityCreator().CreateMap();
    
    // Directional light.
    Entity* dirLight = CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.01f, 0.01f, 0.01f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.2f;
    
    postProcessing = new PostProcessing(MainWindow::GetInstance()->GetSize());
    fxaaFilter = new FXAAFilter();
    gammaCorrectionFilter = new GammaCorrectionFilter();
}

DanielScene::~DanielScene() {
    delete fxaaFilter;
    delete gammaCorrectionFilter;
    delete postProcessing;
}

void DanielScene::Update(float deltaTime) {
    // ControllerSystem
    mControllerSystem.Update(*this, deltaTime);
    
    // PhysicsSystem.
    mPhysicsSystem.Update(*this, deltaTime);
    
    // UpdateCamera
    UpdateCamera(mMainCamera, mPlayers);
    
    for (auto player : mPlayers) {
        GridCollide(player, deltaTime);
        if (player->GetComponent<Component::Health>()->health < 0.01f) {
            player->GetComponent<Component::Physics>()->velocity.x = -10.f;
            player->GetComponent<Component::Health>()->health = player->GetComponent<Component::Health>()->maxHealth;
        }
    }

    // ParticleSystem
    mParticleSystem.Update(*this, deltaTime);
    
    // Updates model matrices for this frame.
    UpdateModelMatrices();
    
    // Check collisions.
    mCollisionSystem.Update(*this);
    
    // Update health
    mHealthSystem.Update(*this, deltaTime);
    
    // Update damage
    mDamageSystem.Update(*this);
    
    // Update lifetimes
    mLifeTimeSystem.Update(*this, deltaTime);
    
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

bool DanielScene::GridCollide(Entity* entity, float deltaTime) {
    
    Component::Transform* transform = entity->GetComponent<Component::Transform>();
    Component::Physics* physics = entity->GetComponent<Component::Physics>();
    
    float x = transform->position.x + (25.f / 2.f) * 10;
    float z = transform->position.z + (25.f / 2.f) * 10;
    z = (250 - z) / 10 + 0.4f;
    x = x / 10 + 0.4f;
    
    if (GameObject::Cave::mMap[(int)x][(int)z]) {
        float oldX = x - physics->velocity.x * deltaTime;
        float oldZ = z + physics->velocity.z * deltaTime;
        if (glm::abs(physics->velocity.x) < glm::abs(physics->velocity.z)) {
            if ((int)x != (int)oldX) {
                transform->position -= glm::vec3((int)x - (int)oldX, 0, 0);
                physics->velocity = glm::vec3(-physics->velocity.x, 0, physics->velocity.z);
                physics->acceleration = -glm::normalize(physics->acceleration);
            } else if ((int)z != (int)oldZ) {
                transform->position += glm::vec3(0, 0, (int)z - (int)oldZ);
                physics->velocity = glm::vec3(physics->velocity.x, 0, -physics->velocity.z);
                physics->acceleration = -glm::normalize(physics->acceleration);
            }
        } else {
            if ((int)z != (int)oldZ) {
                transform->position += glm::vec3(0, 0, (int)z - (int)oldZ);
                physics->velocity = glm::vec3(physics->velocity.x, 0, -physics->velocity.z);
                physics->acceleration = -glm::normalize(physics->acceleration);
            }
            else if ((int)x != (int)oldX) {
                transform->position -= glm::vec3((int)x - (int)oldX, 0, 0);
                physics->velocity = glm::vec3(-physics->velocity.x, 0, physics->velocity.z);
                physics->acceleration = -glm::normalize(physics->acceleration);
            }
        }
        return true;
    }
    return false;
}
