#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>

#include <Geometry/Cube.hpp>
#include <Resources.hpp>

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include "CaveSystem/CaveSystem.hpp"
#include "../Game/Component/Controller.hpp"

#include <Util/FileSystem.hpp>
#include <Util/Input.hpp>

//#include <crtdbg.h>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
#include <System/ParticleSystem.hpp>
#include <System/ParticleRenderSystem.hpp>
#include <PostProcessing/PostProcessing.hpp>

#include "Game/System/HealthSystem.hpp"
#include "Game/System/DamageSystem.hpp"
#include "Game/System/ControllerSystem.hpp"
#include "Util/CameraUpdate.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "Game/Util/GameEntityFactory.hpp"

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/DirectionalLight.hpp>
#include <Component/SpotLight.hpp>
#include <Component/Physics.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/ParticleEmitter.hpp>
#include "Component/Health.hpp"

#include <Texture/Texture2D.hpp>

#include <thread>
#include <fstream>
#include "Util/ControlSchemes.hpp"

using namespace std;

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
    
    // Particle System.
    System::ParticleSystem* particleSystem;
    particleSystem = new System::ParticleSystem;
    particleSystem->SetActive();
    
    // Particle texture.
    Texture2D* particleTexture;
    particleTexture = Resources().CreateTexture2DFromFile("Resources/DustParticle.png");
    
    // RenderSystem.
    System::RenderSystem renderSystem;
    
    // Post-processing swap chain.
    PostProcessing* postProcessing = new PostProcessing(window->GetSize());
    
    // Scene and Entites. 
    Scene scene;
    
    // PhysicsSystem.
    System::PhysicsSystem physicsSystem;
    
    // ControllerSystem.
    System::ControllerSystem controllerSystem;
    
    // HealthSystem.
    System::HealthSystem healthSystem;
    
    // DamageSystem.
    System::DamageSystem damageSystem;
    
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
    
    GameEntityCreator().SetScene(&scene);
    
    // CollisionSystem.
    System::CollisionSystem collisionSystem;
    
    Entity* mainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    mainCamera->AddComponent<Component::Physics>();
    
    Entity* player1 = GameEntityCreator().CreatePlayer(glm::vec3(-4.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    Entity* player2 = GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_TWO);
    std::vector<Entity*> players;
    players.push_back(player1);
    players.push_back(player2);

    Entity* theMap = GameEntityCreator().CreateMap();
    theMap->GetComponent<Component::Transform>()->Rotate(90, 180, 0);
    theMap->GetComponent<Component::Transform>()->scale = glm::vec3(10, 10, 10);
    theMap->GetComponent<Component::Transform>()->Move(glm::vec3(1.f, 0, -1.f));
    
    // Create dust particles
    GameEntityCreator().CreatePointParticle(player1, particleTexture);
    GameEntityCreator().CreatePointParticle(player2, particleTexture);
    GameEntityCreator().CreateCuboidParticle(player1, particleTexture);
    
    // Test texture
    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");
    
    // Directional light.
    Entity* dirLight = scene.CreateEntity();
    Component::Transform* transform = dirLight->AddComponent<Component::Transform>();
    transform->pitch = 90.f;
    Component::DirectionalLight* dLight = dirLight->AddComponent<Component::DirectionalLight>();
    dLight->color = glm::vec3(0.1f, 0.1f, 0.1f);
    dLight->ambientCoefficient = 0.2f;

    // Spot light.
    Entity* spotLight = scene.CreateEntity();
    spotLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    spotLight->GetComponent<Component::RelativeTransform>()->parentEntity = player1;
    spotLight->GetComponent<Component::RelativeTransform>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    spotLight->AddComponent<Component::Mesh>()->geometry = player1->GetComponent<Component::Mesh>()->geometry;

    spotLight->AddComponent<Component::SpotLight>()->coneAngle = 90;
    spotLight->GetComponent<Component::SpotLight>()->attenuation = 0.1f;

    spotLight->AddComponent<Component::Physics>();
    spotLight->AddComponent<Component::Controller>()->playerID = InputHandler::PLAYER_ONE;
    spotLight->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::StickRotate);

    // Spot light.
    Entity* spotLight2 = scene.CreateEntity();
    spotLight2->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    spotLight2->GetComponent<Component::RelativeTransform>()->parentEntity = player2;
    spotLight2->GetComponent<Component::RelativeTransform>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    spotLight2->AddComponent<Component::Mesh>()->geometry = player1->GetComponent<Component::Mesh>()->geometry;

    spotLight2->AddComponent<Component::SpotLight>()->coneAngle = 90;
    spotLight2->GetComponent<Component::SpotLight>()->attenuation = 0.1f;

    spotLight2->AddComponent<Component::Physics>();
    spotLight2->AddComponent<Component::Controller>()->playerID = InputHandler::PLAYER_TWO;
    spotLight2->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::StickRotate);


    GameEntityCreator().CreateBasicEnemy(glm::vec3(5, 0, 5));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-20, 0, -10));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-10, 0, -10));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-30, 0, -10));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(5, 0, 20));
    GameEntityCreator().CreateBasicEnemy(glm::vec3(5, 0, 30));

    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));
    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));
    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));
    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));
    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));
    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));
    //GameEntityCreator().CreateBasicEnemy(glm::vec3(2, 0, 0));


    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    Log() << to_string(lastTimeRender) << "\n";
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glm::vec3 p1OldPos = player1->GetComponent<Component::Transform>()->position;
        glm::vec3 p2OldPos = player2->GetComponent<Component::Transform>()->position;
        // ControllerSystem
        controllerSystem.Update(scene, static_cast<float>(deltaTime));
        
        // PhysicsSystem.
        physicsSystem.Update(scene, (float)deltaTime);

        GridCollide(player1, deltaTime);
        GridCollide(player2, deltaTime);

        // UpdateCamera
        UpdateCamera(mainCamera, players);
        
        // ParticleSystem
        particleSystem->Update(scene, deltaTime);
        
        // Updates model matrices for this frame.
        scene.UpdateModelMatrices();
        
        // Check collisions.
        collisionSystem.Update(scene);
        
        // Update health
        healthSystem.Update(scene, static_cast<float>(deltaTime));
        
        // Update damage
        damageSystem.Update(scene);
        
        // Render.
        renderSystem.Render(scene, postProcessing->GetRenderTarget());
        postProcessing->Render();
        
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
    Resources().FreeTexture2DFromFile(particleTexture);
    Resources().FreeCube();
    Resources().FreeCube();
    
    delete postProcessing;
    delete window;
    delete particleSystem;
    
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

    if (Caves::CaveSystem::mMap[(int)x][(int)z]) {

        float oldX = x - physics->velocity.x * deltaTime;
        float oldZ = z + physics->velocity.z * deltaTime;

        if (glm::abs(physics->velocity.x) < glm::abs(physics->velocity.z)) {

            if ((int)x != (int)oldX) {
                transform->position -= glm::vec3((int)x - (int)oldX, 0, 0);//(glm::vec3(-physics->velocity.x, 0, physics->velocity.z) * (float)deltaTime) * 2.f;
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
                transform->position -= glm::vec3((int)x - (int)oldX, 0, 0);//(glm::vec3(-physics->velocity.x, 0, physics->velocity.z) * (float)deltaTime) * 2.f;
                physics->velocity = glm::vec3(-physics->velocity.x, 0, physics->velocity.z);
                physics->acceleration = -glm::normalize(physics->acceleration);
            }

        }



        return true;

    }

    return false;

}
