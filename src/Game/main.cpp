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
#include <Component/ParticleEmitter.hpp>

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

    // Particle System
    ParticleSystem* particleSystem;
    Texture2D* particleTexture;

    Caves::CaveSystem testCaveSystem(&scene);

    Entity* map = testCaveSystem.GenerateCaveSystem();
    map->GetComponent<Component::Transform>()->scale = glm::vec3(1.8f, 1.8f, 1.8f);
    map->AddComponent<Component::Physics>()->angularVelocity.y = 0.1f;
    map->GetComponent<Component::Physics>()->angularDragFactor = 0.f;
    
    Entity* cameraEntity = scene.CreateEntity();
    cameraEntity->AddComponent<Component::Lens>();
    cameraEntity->AddComponent<Component::Transform>();
    cameraEntity->AddComponent<Component::ParticleEmitter>();
    
    cameraEntity->GetComponent<Component::ParticleEmitter>()->emitterType = Component::ParticleEmitter::CUBOID;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->follow = cameraEntity;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->maxEmitTime = 0.02;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->minEmitTime = 0.01;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->timeToNext = cameraEntity->GetComponent<Component::ParticleEmitter>()->minEmitTime +((double)rand() / RAND_MAX) * (cameraEntity->GetComponent<Component::ParticleEmitter>()->maxEmitTime - cameraEntity->GetComponent<Component::ParticleEmitter>()->minEmitTime);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->lifetime = 0.0;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->origin = glm::vec3(0.f, 0.f, 0.f);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->size = glm::vec3(40.f, 15.f, 40.f);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->relative = true;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->follow = cameraEntity;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->timeToNext = 5.0;

    cameraEntity->GetComponent<Component::Transform>()->Move(0.f, 35.f, 35.f);
    cameraEntity->GetComponent<Component::Transform>()->Rotate(0.f, 50.f, 0.f);

    // Particle texture.
    particleTexture = Resources().CreateTexture2DFromFile("Resources/DustParticle.png");

    //Particle type.
    ParticleType dustParticle;
    dustParticle.texture = particleTexture;
    dustParticle.mMinLifetime = 6.f;
    dustParticle.mMaxLifetime = 10.f;
    dustParticle.mMinVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    dustParticle.mMaxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    dustParticle.mMinSize = glm::vec2(0.025f, 0.025f);
    dustParticle.mMaxSize = glm::vec2(0.05f, 0.05f);
    dustParticle.mUniformScaling = true;
    dustParticle.mColor = glm::vec3(.3f, .3f, 1.f);


    particleSystem = new ParticleSystem(dustParticle, 1000);
    particleSystem->AddParticleEmitter(cameraEntity->GetComponent<Component::ParticleEmitter>());
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleSystem = particleSystem;

    // Test texture
    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        // PhysicsSystem.
        physicsSystem.Update(scene, deltaTime);

        // Update ParticleSystem
        particleSystem->Update(deltaTime, cameraEntity);

        // Updates model matrices for this frame.
        scene.UpdateModelMatrices();

        // Render.
        renderSystem.Render(scene);

        // Input testing.
        window->Update();

        // Render particle system
        particleSystem->Update(deltaTime, cameraEntity);
        particleSystem->Render(cameraEntity, window->GetSize());
        
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
    Resources().FreeTexture2DFromFile(particleTexture);
    Resources().FreeCube();
    
    delete window;
    delete particleSystem;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    return 0;
}
