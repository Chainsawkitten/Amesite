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

#include "Game/System/ControllerSystem.hpp"
#include "Game/System/CameraSystem.hpp"

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
//#include <Component/Collider2DRectangle.hpp>

#include <Texture/Texture2D.hpp>

#include <thread>
#include <fstream>

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

    // RenderSystem.
    System::RenderSystem renderSystem;

    // Scene and Entites. 
    Scene scene;

    // PhysicsSystem.
    System::PhysicsSystem physicsSystem;

    // ControllerSystem
    System::ControllerSystem controllerSystem;

    // CameraSystem
    System::CameraSystem cameraSystem;

    Input()->AssignJoystick(Input()->MOVE_X, true, Input()->LEFT_STICK_X, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->MOVE_Z, true, Input()->LEFT_STICK_Y, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->AIM_X, true, Input()->RIGHT_STICK_Y, Input()->PLAYER_ONE);
    Input()->AssignJoystick(Input()->AIM_Z, true, Input()->RIGHT_STICK_X, Input()->PLAYER_ONE);

    Input()->AssignJoystick(Input()->MOVE_X, true, Input()->LEFT_STICK_X, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->MOVE_Z, true, Input()->LEFT_STICK_Y, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->AIM_X, true, Input()->RIGHT_STICK_Y, Input()->PLAYER_TWO);
    Input()->AssignJoystick(Input()->AIM_Z, true, Input()->RIGHT_STICK_X, Input()->PLAYER_TWO);

    GameEntityCreator().SetScene(&scene);

    int score = 0;
    time_t startTime = time(nullptr);
    int session = 0;

    // CollisionSystem.
    System::CollisionSystem collisionSystem;

    Entity* mainCamera = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    Entity* theJoker = GameEntityCreator().CreateBasicEnemy(glm::vec3(-5.f, -5.f, -5.f));
    Entity* player = GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_ONE);
    Entity* player2 = GameEntityCreator().CreatePlayer(glm::vec3(0.f, 0.f, 0.f), InputHandler::PLAYER_TWO);
    Entity* theMap = GameEntityCreator().CreateMap();

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
    transform = spotLight->AddComponent<Component::Transform>();
    transform->position = glm::vec3(0.f, 1.f, 0.f);
    transform->yaw = 90.f;
    Component::SpotLight* sLight = spotLight->AddComponent<Component::SpotLight>();
    sLight->color = glm::vec3(1.f, 1.f, 1.f);
    sLight->attenuation = 0.1f;
    sLight->coneAngle = 30.f;
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    Log() << to_string(lastTimeRender);
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        // ControllerSystem
        controllerSystem.Update(scene, deltaTime);

        // CameraSystem
        cameraSystem.Update(scene, deltaTime);

        // PhysicsSystem.
        physicsSystem.Update(scene, (float)deltaTime);

        // Updates model matrices for this frame.
        scene.UpdateModelMatrices();

        // Check collisions.
        collisionSystem.Update(scene);

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
    Resources().FreeCube();
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