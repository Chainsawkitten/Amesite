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

    Caves::CaveSystem testCaveSystem(&scene);
    testCaveSystem.GenerateCaveSystem();
    
    Entity* cubeEntity = scene.CreateEntity();
    cubeEntity->AddComponent<Component::Mesh>();
    cubeEntity->AddComponent<Component::Transform>();
    cubeEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    Entity* cubeChildEntity = scene.CreateEntity();
    cubeChildEntity->AddComponent<Component::Mesh>()->geometry = cubeEntity->GetComponent<Component::Mesh>()->geometry;
    cubeChildEntity->AddComponent<Component::RelativeTransform>()->parentEntity = cubeEntity;
    cubeChildEntity->GetComponent<Component::RelativeTransform>()->Move(1.f, 1.f, -1.f);
    
    Entity* cameraEntity = scene.CreateEntity();
    cameraEntity->AddComponent<Component::Lens>();
    cameraEntity->AddComponent<Component::Transform>();

    cameraEntity->GetComponent<Component::Transform>()->Move(12.5f, -12.5f, 35.f);
    cameraEntity->GetComponent<Component::Transform>()->Rotate(0.f, 0.f, 0.f);

    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        lastTime = glfwGetTime();
        
        // Move cube.
        cubeEntity->GetComponent<Component::Transform>()->Rotate(1.f, 0.f, 0.f);
        
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
