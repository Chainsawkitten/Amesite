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

#include <Texture/Texture2D.hpp>

#include <thread>
#include <fstream>
#include "Util/ControlSchemes.hpp"

using namespace std;

std::string space2underscore(std::string text);

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
    
    Input()->AssignJoystick(InputHandler::MOVE_X, true, InputHandler::LEFT_STICK_X, InputHandler::PLAYER_ONE);
    Input()->AssignJoystick(InputHandler::MOVE_Z, true, InputHandler::LEFT_STICK_Y, InputHandler::PLAYER_ONE);
    Input()->AssignJoystick(InputHandler::AIM_X, true, InputHandler::RIGHT_STICK_X, InputHandler::PLAYER_ONE);
    Input()->AssignJoystick(InputHandler::AIM_Z, true, InputHandler::RIGHT_STICK_Y, InputHandler::PLAYER_ONE);
    Input()->AssignJoystick(InputHandler::SHOOT, false, InputHandler::RIGHT_BUMPER, InputHandler::PLAYER_ONE);
    
    Input()->AssignJoystick(InputHandler::MOVE_X, true, InputHandler::LEFT_STICK_X, InputHandler::PLAYER_TWO);
    Input()->AssignJoystick(InputHandler::MOVE_Z, true, InputHandler::LEFT_STICK_Y, InputHandler::PLAYER_TWO);
    Input()->AssignJoystick(InputHandler::AIM_X, true, InputHandler::RIGHT_STICK_X, InputHandler::PLAYER_TWO);
    Input()->AssignJoystick(InputHandler::AIM_Z, true, InputHandler::RIGHT_STICK_Y, InputHandler::PLAYER_TWO);
    Input()->AssignJoystick(InputHandler::SHOOT, false, InputHandler::RIGHT_BUMPER, InputHandler::PLAYER_TWO);
    
    Input()->AssignKeyboard(InputHandler::UP, GLFW_KEY_W, InputHandler::PLAYER_ONE);
    Input()->AssignKeyboard(InputHandler::DOWN, GLFW_KEY_S, InputHandler::PLAYER_ONE);
    Input()->AssignKeyboard(InputHandler::RIGHT, GLFW_KEY_D, InputHandler::PLAYER_ONE);
    Input()->AssignKeyboard(InputHandler::LEFT, GLFW_KEY_A, InputHandler::PLAYER_ONE);
    //Input()->AssignKeyboard(InputHandler::SHOOT, GLFW_KEY_T, InputHandler::PLAYER_ONE);
    
    GameEntityCreator().SetScene(&scene);
    
    int score = 0;
    time_t startTime = time(nullptr);
    int session = 0;
    
    // CollisionSystem.
    System::CollisionSystem collisionSystem;
    
    Entity* mainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    mainCamera->AddComponent<Component::Physics>();
    GameEntityCreator().CreateBasicEnemy(glm::vec3(-5.f, -5.f, -5.f));
    
    Entity* player1 = GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    Entity* player2 = GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_TWO);
    std::vector<Entity*> players;
    players.push_back(player1);
    players.push_back(player2);

    Entity* theMap = GameEntityCreator().CreateMap();
    theMap->GetComponent<Component::Transform>()->Rotate(90, 180, 0);
    theMap->GetComponent<Component::Transform>()->scale = glm::vec3(10, 10, 10);
    theMap->GetComponent<Component::Transform>()->Move(glm::vec3(1.f, 0, -1.f));
    
    GameEntityCreator().CreateBullet(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
    
    // Create dust particles
    GameEntityCreator().CreateCuboidParticle(mainCamera, particleTexture);
    
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
    spotLight->GetComponent<Component::Controller>()->ControlScheme = &ControlScheme::StickRotate;
    

    int p1OldGridPosX = 12;
    int p1OldGridPosZ = 12;

    int p2OldGridPosX = 12;
    int p2OldGridPosZ = 12;


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

        float p1X = player1->GetComponent<Component::Transform>()->position.x + (25.f / 2.f) * 10;
        float p1Z = player1->GetComponent<Component::Transform>()->position.z + (25.f / 2.f) * 10;
        p1Z = (250 - p1Z) / 10 + 0.4f;
        p1X = p1X / 10 + 0.4f;

        float p2X = player2->GetComponent<Component::Transform>()->position.x + (25.f / 2.f) * 10;
        float p2Z = player2->GetComponent<Component::Transform>()->position.z + (25.f / 2.f) * 10;
        p2Z = (250 - p2Z) / 10 + 0.4f;
        p2X = p2X / 10 + 0.4f;

        bool p1Collide = Caves::CaveSystem::mMap[(int)p1X][(int)p1Z] == 1;
        bool p2Collide = Caves::CaveSystem::mMap[(int)p2X][(int)p2Z] == 1;

        if (p1Collide) {

            if (p1OldGridPosX != (int)p1X) {

                glm::vec3 velocity = player1->GetComponent<Component::Physics>()->velocity;

                player1->GetComponent<Component::Transform>()->position = p1OldPos + (glm::vec3(-velocity.x, 0, velocity.z) * (float)deltaTime);
                player1->GetComponent<Component::Physics>()->velocity = glm::vec3(-velocity.x, 0, velocity.z);//-glm::normalize(player1->GetComponent<Component::Physics>()->velocity) * 1000.f;
                player1->GetComponent<Component::Physics>()->acceleration = -glm::normalize(player1->GetComponent<Component::Physics>()->acceleration);

            }
            if (p1OldGridPosZ != (int)p1Z) {

                glm::vec3 velocity = player1->GetComponent<Component::Physics>()->velocity;

                player1->GetComponent<Component::Transform>()->position = p1OldPos + (glm::vec3(velocity.x, 0, -velocity.z) * (float)deltaTime);
                player1->GetComponent<Component::Physics>()->velocity = glm::vec3(velocity.x, 0, -velocity.z);//-glm::normalize(player1->GetComponent<Component::Physics>()->velocity) * 1000.f;
                player1->GetComponent<Component::Physics>()->acceleration = -glm::normalize(player1->GetComponent<Component::Physics>()->acceleration);

            }


        }
        if (p2Collide) {

            if (p2OldGridPosX != (int)p2X) {

                glm::vec3 velocity = player2->GetComponent<Component::Physics>()->velocity;

                player2->GetComponent<Component::Transform>()->position = p2OldPos + (glm::vec3(-velocity.x, 0, velocity.z) * (float)deltaTime);
                player2->GetComponent<Component::Physics>()->velocity = glm::vec3(-velocity.x, 0, velocity.z);//-glm::normalize(player1->GetComponent<Component::Physics>()->velocity) * 1000.f;
                player2->GetComponent<Component::Physics>()->acceleration = -glm::normalize(player2->GetComponent<Component::Physics>()->acceleration);

            }
            if (p2OldGridPosZ != (int)p2Z) {

                glm::vec3 velocity = player2->GetComponent<Component::Physics>()->velocity;

                player2->GetComponent<Component::Transform>()->position = p2OldPos + (glm::vec3(velocity.x, 0, -velocity.z) * (float)deltaTime);
                player2->GetComponent<Component::Physics>()->velocity = glm::vec3(velocity.x, 0, -velocity.z);//-glm::normalize(player1->GetComponent<Component::Physics>()->velocity) * 1000.f;
                player2->GetComponent<Component::Physics>()->acceleration = -glm::normalize(player2->GetComponent<Component::Physics>()->acceleration);

            }


        }

        p1OldGridPosX = (int)p1X;
        p1OldGridPosZ = (int)p1Z;
        p2OldGridPosX = (int)p2X;
        p2OldGridPosZ = (int)p2Z;
        


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
    Resources().FreeTexture2DFromFile(particleTexture);
    Resources().FreeCube();
    Resources().FreeCube();
    
    delete window;
    delete particleSystem;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    return 0;
}

std::string space2underscore(std::string text) {
    for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
        if (*it == ' ') {
            *it = '_';
        }
        else if (*it == ':') {
            *it = '-';
        }
    }
    return text;
}
