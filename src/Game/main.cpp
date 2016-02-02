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
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>
#include <Component/ParticleEmitter.hpp>

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

    // Particle System
    ParticleSystem* particleSystem;
    Texture2D* particleTexture;

    // Particle texture.
    particleTexture = Resources().CreateTexture2DFromFile("Resources/DustParticle.png");

    Caves::CaveSystem testCaveSystem(&scene);
    testCaveSystem.GenerateCaveSystem();
    
    Entity* cubeEntity = scene.CreateEntity();
    cubeEntity->AddComponent<Component::Mesh>();
    cubeEntity->AddComponent<Component::Transform>();
    cubeEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    Entity* collisionCubeA = scene.CreateEntity();
    collisionCubeA->AddComponent<Component::Mesh>();
    collisionCubeA->AddComponent<Component::Transform>();
    collisionCubeA->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    collisionCubeA->AddComponent<Component::Collider2DRectangle>();
    collisionCubeA->GetComponent<Component::Transform>()->Move(-4.f, 0.f, -4.f);
    collisionCubeA->GetComponent<Component::Collider2DRectangle>()->height = 1.f;
    collisionCubeA->GetComponent<Component::Collider2DRectangle>()->width = 1.f;

    Entity* collisionCubeB = scene.CreateEntity();
    collisionCubeB->AddComponent<Component::Mesh>();
    collisionCubeB->AddComponent<Component::Transform>();
    collisionCubeB->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    collisionCubeB->AddComponent<Component::Collider2DCircle>();
    collisionCubeB->GetComponent<Component::Transform>()->Move(-4.f,0.f,-6.f);
    collisionCubeB->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;

    Entity* cubeChildEntity = scene.CreateEntity();
    cubeChildEntity->AddComponent<Component::Mesh>()->geometry = cubeEntity->GetComponent<Component::Mesh>()->geometry;
    cubeChildEntity->AddComponent<Component::RelativeTransform>()->parentEntity = cubeEntity;
    cubeChildEntity->GetComponent<Component::RelativeTransform>()->Move(1.f, 1.f, -1.f);
    
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

    cameraEntity->GetComponent<Component::Transform>()->Move(-5.0f, 12.5f, -5.0f);
    cameraEntity->GetComponent<Component::Transform>()->Rotate(0.f, 90.f, 0.f);

    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");

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

    // Emitters.
    particleSystem->AddParticleEmitter(cameraEntity->GetComponent<Component::ParticleEmitter>());
    cameraEntity->GetComponent<Component::ParticleEmitter>()->Update(5.0, cameraEntity, particleSystem);
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    float rotation = 0;
    glm::vec3 cubeAOrigin = collisionCubeA->GetComponent<Component::Transform>()->position;
    glm::vec3 cubeBOrigin = collisionCubeA->GetComponent<Component::Transform>()->position;
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        rotation += deltaTime;
        if (rotation > 360.f)
            rotation -= 360.f;
        
        // Move cube.
        cubeEntity->GetComponent<Component::Transform>()->Rotate(1.f, 0.f, 0.f);

        // Update ParticleSystem
        particleSystem->Update(deltaTime, cameraEntity);

        // Move collision cubes.
        collisionCubeA->GetComponent<Component::Transform>()->position = cubeAOrigin + glm::vec3(glm::cos(rotation), 0.f, -glm::sin(rotation));
        collisionCubeB->GetComponent<Component::Transform>()->position = cubeBOrigin + glm::vec3(glm::cos(rotation), 0.f, glm::sin(rotation));

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
