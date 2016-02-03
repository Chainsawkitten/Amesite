#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>

#include <Geometry/Cube.hpp>
#include <Resources.hpp>

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
//#include "CaveSystem/CaveSystem.hpp"
#include <Util/FileSystem.hpp>
#include <Util/Input.hpp>

//#include "Engine/Particles/CuboidParticleEmitter.hpp"
//#include "Engine/Particles/PointParticleEmitter.hpp"
//#include "Engine/Particles/ParticleSystem.hpp"

#include "System/RenderSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "Game/Util/GameEntityFactory.hpp"

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>

#include <CollisionSystem/CollisionSystem.hpp>

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

    GameEntityCreator().SetScene(&scene);

    Entity* entity = GameEntityCreator().CreateCamera(glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    entity = GameEntityCreator().CreateBasicEnemy(glm::vec3(0.f, 0.f, 0.f));
	entity = GameEntityCreator().CreateCube(glm::vec3(3.f, 0.f, 3.f));

    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    float rotation = 0;
   // glm::vec3 cubeAOrigin = collisionCubeA->GetComponent<Component::Transform>()->position;
    //glm::vec3 cubeBOrigin = collisionCubeA->GetComponent<Component::Transform>()->position;
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        rotation += deltaTime;
        if (rotation > 360.f)
            rotation -= 360.f;
        
        // Move cube.
        //cubeEntity->GetComponent<Component::Transform>()->Rotate(1.f, 0.f, 0.f);

        // Move collision cubes.
        //collisionCubeA->GetComponent<Component::Transform>()->position = cubeAOrigin + glm::vec3(glm::cos(rotation), 0.f, -glm::sin(rotation));
        //collisionCubeB->GetComponent<Component::Transform>()->position = cubeBOrigin + glm::vec3(glm::cos(rotation), 0.f, glm::sin(rotation));

        // Render.
        renderSystem.Render(scene);

        // Input testing.
        window->Update();
        
        testTexture->Render(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f), window->GetSize());

        //explosionEmitter->Update(15, explosionParticleSystem, cameraEntity);

        //explosionParticleSystem->Render(cameraEntity, window->GetSize());

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
