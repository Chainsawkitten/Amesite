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


#include "System/RenderSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>

#include <Texture/Texture2D.hpp>
#include <Component/RelativeTransform.hpp>

#include <thread>

#include "Player/Player.hpp"

using namespace std;

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
   
    Entity* cubeEntity2 = scene.CreateEntity();
    cubeEntity2->AddComponent<Component::Mesh>();
    cubeEntity2->AddComponent<Component::Transform>();
    cubeEntity2->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

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

    Entity* cameraEntity = scene.CreateEntity();
    cameraEntity->AddComponent<Component::Lens>();
    cameraEntity->AddComponent<Component::Transform>();

    cameraEntity->GetComponent<Component::Transform>()->Move(0.f, -34.5f, 0.f);
    cameraEntity->GetComponent<Component::Transform>()->Rotate(0.f, -90.f, 0.f);

    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");


    glm::vec3 velocity(0.5f, 0, 0);

    Input()->AssignJoystick(Input()->MOVE_X, true, Input()->LEFT_STICK_X, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->MOVE_Z, true, Input()->LEFT_STICK_Y, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->AIM_X, true, Input()->RIGHT_STICK_Y, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->AIM_Z, true, Input()->RIGHT_STICK_X, Input()->PLAYER_ONE);

    Input()->AssignJoystick(Input()->MOVE_X, true, Input()->LEFT_STICK_X, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->MOVE_Z, true, Input()->LEFT_STICK_Y, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->AIM_X, true, Input()->RIGHT_STICK_Y, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->AIM_Z, true, Input()->RIGHT_STICK_X, Input()->PLAYER_TWO);

    Player player(cubeEntity, 20);
    Player player2(cubeEntity2, 20, InputHandler::PLAYER_TWO);

    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        lastTime = glfwGetTime();

        // Move cube.
        //cubeEntity->GetComponent<Component::Transform>()->Rotate(1.f, 0.f, 0.f);
        cubeEntity->GetComponent<Component::Transform>()->Move(velocity);

        player.Update(0.01f);
        player2.Update(0.01f);

        float a = Input()->ButtonValue(Input()->AIM_X, Input()->PLAYER_TWO);
        float b = Input()->ButtonValue(Input()->AIM_Z, Input()->PLAYER_TWO);

        if(glm::abs(a) + glm::abs(b) > 0.2f)
            if(a > 0)
                turretJoint->GetComponent<Component::RelativeTransform>()->yaw = glm::atan(b / a) * 360.f/(2*3.14);
            else
                turretJoint->GetComponent<Component::RelativeTransform>()->yaw = 180 + glm::atan(b / a) * 360.f / (2 * 3.14);

        Log() << (float) Input()->ButtonValue(Input()->MOVE_X, Input()->PLAYER_ONE) << "\n";

        velocity *= 0.95f;

        // Render.
        renderSystem.Render(scene);

        // Input testing.
        window->Update();
        
        testTexture->Render(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f), window->GetSize());

        // Set window title to reflect screen update and render times.
        std::string title = "Modership";
        if (GameSettings::GetInstance().GetBool("Show Frame Times"))
            title += " - " + std::to_string((glfwGetTime() - lastTime) * 1000.0) + " ms";
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
