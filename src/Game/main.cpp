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

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
#include <System/ParticleSystem.hpp>

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "Game/Util/GameEntityFactory.hpp"

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Physics.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/ParticleEmitter.hpp>
//#include <Component/Collider2DRectangle.hpp>

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

    // CollisionSystem.
    System::CollisionSystem collisionSystem;

    // Scene and Entites. 
    Scene scene;

    // Particle System
    /*System::ParticleSystem particleSystem;
    Texture2D* particleTexture;

    cameraEntity->AddComponent<Component::ParticleEmitter>();
    
    // Particle emitter.
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
    
    // Particle texture.
    particleTexture = Resources().CreateTexture2DFromFile("Resources/DustParticle.png");

    //Particle type.
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.texture = particleTexture;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime = 6.f;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime = 10.f;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.minVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.maxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.minSize = glm::vec2(0.025f, 0.025f);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.maxSize = glm::vec2(0.05f, 0.05f);
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.uniformScaling = true;
    cameraEntity->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(.3f, .3f, 1.f);*/
    
    
    GameEntityCreator().SetScene(&scene);

    Entity* entity = GameEntityCreator().CreateCamera(glm::vec3(0.f, 40.f, 0.f), glm::vec3(0.f, 90.f, 0.f));
    entity = GameEntityCreator().CreateBasicEnemy(glm::vec3(-5.f, -5.f, -5.f));
    Caves::CaveSystem* theMap = GameEntityCreator().CreateMap();

    // Test texture
    Texture2D* testTexture = Resources().CreateTexture2DFromFile("Resources/TestTexture.png");
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    float rotation = 0;
   
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        // PhysicsSystem.
        physicsSystem.Update(scene, (float)deltaTime);

        // Updates model matrices for this frame.
        scene.UpdateModelMatrices();

        // Check collisions
        collisionSystem.Update(scene);

        // Render.
        renderSystem.Render(scene);

        // Input testing.
        window->Update();

        // Render particle system
        //particleSystem->Update(deltaTime, cameraEntity);
        //particleSystem->Render(cameraEntity, window->GetSize());
        
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
    //Resources().FreeTexture2DFromFile(particleTexture);
    Resources().FreeCube();
    
    delete window;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    return 0;
}
