#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>

#include <Texture/Texture2D.hpp>
#include <Geometry/Cube.hpp>
#include <Resources.hpp>

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include "Util/ControlSchemes.hpp"
#include "Util/GameEntityFactory.hpp"
#include "Util/CameraUpdate.hpp"
#include <Util/FileSystem.hpp>
#include <Util/Input.hpp>

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
#include <System/ParticleSystem.hpp>
#include "Game/System/HealthSystem.hpp"
#include "Game/System/DamageSystem.hpp"
#include "Game/System/ControllerSystem.hpp"
#include "Game/System/LifeTimeSystem.hpp"

#include <Component/Transform.hpp>
#include <Component/DirectionalLight.hpp>
#include <Component/Physics.hpp>
#include <Component/ParticleEmitter.hpp>
#include "../Game/Component/Health.hpp"

#include "Game/GameObject/Bullet.hpp"
#include "Game/GameObject/Player.hpp"
#include "Game/GameObject/Camera.hpp"
#include "Game/GameObject/Enemy.hpp"
#include "Game/GameObject/Cave.hpp"

#include <thread>
#include <vector>
//#include <crtdbg.h>

bool GridCollide(Entity* entity, float deltaTime);

int main() {
    
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    // Enable logging if requested.
    if (GameSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::SavePath("Modership", "GameLog.txt").c_str(), "a", stderr);
    
    Log() << "Game started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    MainWindow* window = new MainWindow(GameSettings::GetInstance().GetLong("Screen Width"), GameSettings::GetInstance().GetLong("Screen Height"), GameSettings::GetInstance().GetBool("Fullscreen"), GameSettings::GetInstance().GetBool("Borderless"), "Modership", GameSettings::GetInstance().GetBool("Debug Context"));
    glewInit();
    window->Init();
    window->SetVsync(GameSettings::GetInstance().GetBool("VSync"));
    
    // Scene and Entites. 
    Scene scene;

    System::ParticleSystem particleSystem;
    particleSystem.SetActive();
    System::RenderSystem renderSystem;
    System::PhysicsSystem physicsSystem;
    System::ControllerSystem controllerSystem;
    System::HealthSystem healthSystem;
    System::DamageSystem damageSystem;
    System::CollisionSystem collisionSystem;
    System::LifeTimeSystem lifeTimeSystem;
    
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_X, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::MOVE_Z, InputHandler::JOYSTICK, InputHandler::LEFT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_X, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_X, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::AIM_Z, InputHandler::JOYSTICK, InputHandler::RIGHT_STICK_Y, true);
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::SHOOT, InputHandler::JOYSTICK, InputHandler::RIGHT_BUMPER);
    
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::UP, InputHandler::KEYBOARD, GLFW_KEY_W);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::DOWN, InputHandler::KEYBOARD, GLFW_KEY_S);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_D);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_A);
    Input()->AssignButton(InputHandler::PLAYER_TWO, InputHandler::SHOOT, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_1);
    
    GameEntityCreator().SetScene(&scene);

    std::vector<Entity*> players;
    GameObject::Player* player1 = GameEntityCreator().CreatePlayer(glm::vec3(-4.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    GameObject::Player* player2 = GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_TWO);
    GameEntityCreator().CreatePointParticle(player1->GetEntity("body"), Component::ParticleEmitter::DUST);
    GameEntityCreator().CreatePointParticle(player2->GetEntity("body"), Component::ParticleEmitter::DUST);
    GameEntityCreator().CreateCuboidParticle(player1->GetEntity("body"), Component::ParticleEmitter::DUST);
    players.push_back(player1->GetEntity("body"));
    players.push_back(player2->GetEntity("body"));
    

    GameObject::Camera* mainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));

    GameObject::Cave* map = GameEntityCreator().CreateMap();
    
    // Test texture
    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");
    
    // Directional light.
    Entity* dirLight = scene.CreateEntity();
    dirLight->AddComponent<Component::Transform>()->pitch = 90.f;
    dirLight->AddComponent<Component::DirectionalLight>();
    dirLight->GetComponent<Component::DirectionalLight>()->color = glm::vec3(0.1f, 0.1f, 0.1f);
    dirLight->GetComponent<Component::DirectionalLight>()->ambientCoefficient = 0.2f;

    GameEntityCreator().CreateBasicEnemy(glm::vec3(5, 0, 5));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-20, 0, -10));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-10, 0, -10));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-30, 0, -10));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(5, 0, 20));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(5, 0, 30));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));

    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    Log() << std::to_string(lastTimeRender) << "\n";
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        // Update Scene
        controllerSystem.Update(scene, static_cast<float>(deltaTime));
        physicsSystem.Update(scene, (float)deltaTime);
        //TODO Animation
        scene.UpdateModelMatrices();
        particleSystem.Update(scene, deltaTime);
        collisionSystem.Update(scene);
        healthSystem.Update(scene, static_cast<float>(deltaTime));
        damageSystem.Update(scene);
        lifeTimeSystem.Update(scene, static_cast<float>(deltaTime));

        // Update game logic
        UpdateCamera(mainCamera->GetEntity("body"), players);
        for (auto player : players) {
            GridCollide(player, deltaTime);
            if (player->GetComponent<Component::Health>()->health < 0.01f) {
                player->GetComponent<Component::Physics>()->velocity.x = -10.f;
                player->GetComponent<Component::Health>()->health = player->GetComponent<Component::Health>()->maxHealth;
            }
        }

        // Render scene
        renderSystem.Render(scene);
        
        // Input testing.
        window->Update();
        
        testTexture->Render(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f));
        
        // Set window title to reflect screen update and render times.
        std::string title = "Modership";
        if (GameSettings::GetInstance().GetBool("Show Frame Times"))
            title += " - " + std::to_string((glfwGetTime() - lastTime) * 1000.0f) + " ms";
        window->SetTitle(title.c_str());
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        long wait = static_cast<long>((1.0 / GameSettings::GetInstance().GetLong("Target FPS") + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        glfwPollEvents();
    }
    
    Resources().FreeTexture2DFromFile(testTexture);
    
    delete window;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    return 0;
}

bool GridCollide(Entity* entity, float deltaTime) {

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
            }
            else if ((int)z != (int)oldZ) {
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