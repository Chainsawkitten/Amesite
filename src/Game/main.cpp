#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>

#include <Geometry/Cube.hpp>
#include <Resources.hpp>

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include "CaveSystem/CaveSystem.hpp"
#include <Util/FileSystem.hpp>
#include <Util/Input.hpp>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Physics.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>

#include <Texture/Texture2D.hpp>

#include <thread>

#include "Player/Player.hpp"
#include "Turret/Turret.hpp"

#include <fstream>

using namespace std;

std::string space2underscore(std::string text);

int main() {
    
    // Enable logging if requested.
    if (GameSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::SavePath("Modership", "GameLog.txt").c_str(), "a", stderr);

    Log() << "Game started - " << time(nullptr) << "\n";

    if (!glfwInit())
        return 1;

    MainWindow* window = new MainWindow(GameSettings::GetInstance().GetLong("Screen Width"), GameSettings::GetInstance().GetLong("Screen Height"), GameSettings::GetInstance().GetBool("Fullscreen"), GameSettings::GetInstance().GetBool("Borderless"), "Modership", GameSettings::GetInstance().GetBool("Debug Context"));
    glewInit();
    window->Init();

    // RenderSystem.
    System::RenderSystem renderSystem;

    // Scene and Entites. 
    Scene scene;

    Entity* cubeEntity = scene.CreateEntity();
    cubeEntity->AddComponent<Component::Mesh>();
    cubeEntity->AddComponent<Component::Transform>();
    cubeEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    cubeEntity->AddComponent<Component::Physics>();
    cubeEntity->GetComponent<Component::Transform>()->position = glm::vec3(25, 0, 15);

    Entity* turretJoint = scene.CreateEntity();
    turretJoint->AddComponent<Component::RelativeTransform>();
    turretJoint->GetComponent<Component::RelativeTransform>()->parentEntity = cubeEntity;

    Entity* turretEntity = scene.CreateEntity();
    turretEntity->AddComponent<Component::Mesh>();
    turretEntity->AddComponent<Component::RelativeTransform>();
    turretEntity->GetComponent<Component::RelativeTransform>()->parentEntity = turretJoint;
    turretEntity->GetComponent<Component::RelativeTransform>()->Move(1, 0, 0);
    turretEntity->GetComponent<Component::RelativeTransform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);

    turretEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    // PhysicsSystem.
    System::PhysicsSystem physicsSystem;

    Caves::CaveSystem testCaveSystem(&scene);

    Entity* map = testCaveSystem.GenerateCaveSystem();
    map->GetComponent<Component::Transform>()->scale = glm::vec3(5.f, 5.f, 5.f);
    map->GetComponent<Component::Transform>()->Rotate(0, 0, 180);
    map->GetComponent<Component::Transform>()->Rotate(-90, 0, 180);


    map->AddComponent<Component::Physics>();
    map->GetComponent<Component::Physics>();

    Entity* cameraEntity = scene.CreateEntity();
    cameraEntity->AddComponent<Component::Lens>();
    cameraEntity->AddComponent<Component::RelativeTransform>()->parentEntity = cubeEntity;

    Input()->AssignJoystick(Input()->MOVE_X, true, Input()->LEFT_STICK_X, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->MOVE_Z, true, Input()->LEFT_STICK_Y, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->AIM_X, true, Input()->RIGHT_STICK_Y, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->AIM_Z, true, Input()->RIGHT_STICK_X, Input()->PLAYER_ONE);

    Input()->AssignJoystick(Input()->MOVE_X, true, Input()->LEFT_STICK_X, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->MOVE_Z, true, Input()->LEFT_STICK_Y, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->AIM_X, true, Input()->RIGHT_STICK_Y, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->AIM_Z, true, Input()->RIGHT_STICK_X, Input()->PLAYER_TWO);

    Turret turret(turretJoint, turretEntity);

    cameraEntity->GetComponent<Component::Transform>()->Move(0.f, 50.f, 20.f);
    cameraEntity->GetComponent<Component::Transform>()->Rotate(0.f, 70.f, 0.f);

    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");

    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();

    int score = 0;
    time_t startTime = time(nullptr);
    int session = 0;

    float playerAcceleration = 6000;
    float playerMaxSpeed = 40;
    float playerDrag = 1.5f;

    Player player(cubeEntity, playerAcceleration);
    cubeEntity->GetComponent<Component::Physics>()->maxVelocity = playerMaxSpeed;
    cubeEntity->GetComponent<Component::Physics>()->velocityDragFactor = playerDrag;
    player.SetTurret(&turret);

    std::string testLog = "Player acceleration: " + std::to_string(playerAcceleration) + "\n";
    testLog += "Player max velocity: " + std::to_string(playerMaxSpeed) + "\n";
    testLog += "Player drag: " + std::to_string(playerDrag) + "\n";

    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        int xPos = (int)(cubeEntity->GetComponent<Component::Transform>()->position[0] / 5 + 25.f / 2.f + 0.5f);
        int zPos = (int)(cubeEntity->GetComponent<Component::Transform>()->position[2] / 5 + 25.f / 2.f + 0.25f);

        float caveCollide = *testCaveSystem.mMap[xPos, zPos];

        if (testCaveSystem.mMap[xPos][zPos] == 1.f) {

            cubeEntity->GetComponent<Component::Transform>()->position = glm::vec3(25, 0, 15);
            cubeEntity->GetComponent<Component::Physics>()->velocity = glm::vec3(0, 0, 0);

            Log() << "You died " << "\n";
            testLog += "You died with a score of " + std::to_string(score) + "\n";

            startTime = time(nullptr);
            score = 0;

            for (int i = 0; i < 25; i++) {
                for (int j = 0; j < 25; j++) {
                    if (testCaveSystem.mMap[i][j] == -2.f) {

                        testCaveSystem.mMap[i][j] = -1;

                    }
                }
            }

        }
        if (testCaveSystem.mMap[xPos][zPos] == -1.f) {

            score++;
            Log() << "score! " << "\n";
            testCaveSystem.mMap[xPos][zPos] = -2;

        }

        if (score == 5) {

            Log() << "You win! Your time was: " << (int)(time(nullptr) - startTime) << "\n";
            testLog += "You win! Your time was: " + std::to_string((int)(time(nullptr) - startTime)) + "\n";

            ofstream myfile;
            const time_t now = time(nullptr);
            string fileName(std::asctime(localtime(&now)));
            fileName = fileName.substr(0, fileName.size() - 2);
            fileName += ".txt";
            fileName = space2underscore(fileName);

            Log() << fileName << "\n";

            myfile.open("../logs/Move_Tests/Session_" + std::to_string(session) + "_A-" + std::to_string(playerAcceleration).substr(0, 5) + "_S-" + std::to_string(playerMaxSpeed).substr(0,3) + "_D-" + std::to_string(playerDrag).substr(0,5) + "_" + fileName);
            myfile << testLog;
            myfile.close();

            testLog = "Player acceleration: " + std::to_string(playerAcceleration) + "\n";
            testLog += "Player max velocity: " + std::to_string(playerMaxSpeed) + "\n";
            testLog += "Player drag: " + std::to_string(playerDrag) + "\n";
            score = 0;
            session++;

        }
        
        player.Update(0.01f);
        
        // PhysicsSystem.
        physicsSystem.Update(scene, deltaTime);

        // Updates model matrices for this frame.
        scene.UpdateModelMatrices();

        // Render.
        renderSystem.Render(scene);

        // Input testing.
        window->Update();
        
        testTexture->Render(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f), window->GetSize());

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
    Resources().FreeCube();
    
    delete window;
    
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