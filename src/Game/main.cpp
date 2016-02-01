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
#include <System/RenderSystem.hpp>

#include <Particles/CuboidParticleEmitter.hpp>
#include <Particles/PointParticleEmitter.hpp>
#include <Particles/ParticleSystem.hpp>

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

    // PhysicsSystem.
    System::PhysicsSystem physicsSystem;

    // Scene and Entites. 
    Scene scene;

    Caves::CaveSystem testCaveSystem(&scene);

    Entity* map = testCaveSystem.GenerateCaveSystem();
    map->GetComponent<Component::Transform>()->scale = glm::vec3(1.8f, 1.8f, 1.8f);
    map->AddComponent<Component::Physics>()->angularVelocity.y = 0.1f;
    map->GetComponent<Component::Physics>()->angularDragFactor = 0.f;
    
    Entity* cameraEntity = scene.CreateEntity();
    cameraEntity->AddComponent<Component::Lens>();
    cameraEntity->AddComponent<Component::Transform>();

    cameraEntity->GetComponent<Component::Transform>()->Move(0.f, 35.f, 35.f);
    cameraEntity->GetComponent<Component::Transform>()->Rotate(0.f, 50.f, 0.f);

    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");

    // Particle type.
    ParticleSystem* explosionParticleSystem;
    ParticleEmitter* explosionEmitter;

    ParticleType explosionParticle;
    explosionParticle.texture = testTexture;
    explosionParticle.mMinLifetime = .1f;
    explosionParticle.mMaxLifetime = 2.f;
    explosionParticle.mMinVelocity = glm::vec3(-10.f, 10.f, -10.f);
    explosionParticle.mMaxVelocity = glm::vec3(10.f, -10.f, 10.f);
    explosionParticle.mMinSize = glm::vec2(2.5f, 2.5f);
    explosionParticle.mMaxSize = glm::vec2(5.f, 5.f);
    explosionParticle.mUniformScaling = true;
    explosionParticle.mColor = glm::vec3(1.f, 0.5f, 0.5f);

    explosionParticleSystem = new ParticleSystem(explosionParticle, 1000);
    explosionEmitter = new PointParticleEmitter(cameraEntity->GetComponent<Component::Transform>()->position + glm::vec3(glm::inverse(cameraEntity->GetComponent<Component::Transform>()->GetOrientation()) * glm::vec4(0, 0, -1, 1)), 1, 2, false);
    explosionParticleSystem->AddParticleEmitter(explosionEmitter);
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        // PhysicsSystem.
        physicsSystem.Update(scene, deltaTime);

        // Updates model matrices for this frame.
        scene.UpdateModelMatrices();

        // Render.
        renderSystem.Render(scene);

        // Input testing.
        window->Update();
        
        testTexture->Render(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f), window->GetSize());

        explosionEmitter->Update(15, explosionParticleSystem, cameraEntity);

        explosionParticleSystem->Render(cameraEntity, window->GetSize());

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
