#define NOMINMAX

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/MainWindow.hpp>

#include <Engine/Util/Log.hpp>
#include <Engine/Profiling/CPUProfiling.hpp>
#include "Util/GameSettings.hpp"
#include <Engine/Util/FileSystem.hpp>
#include <Engine/System/SoundSystem.hpp>

#include "Game.hpp"
#include "GameScenes/SplashScene.hpp"
#include "GameScenes/LoadingScene.hpp"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <thread>
#include <vector>
#include <fstream>

using namespace std;

int main() {
    //Enable logging if requested.
    if (GameSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::SavePath("Amesite", "GameLog.txt").c_str(), "a", stderr);
    
    Log() << "Game started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    MainWindow* window = new MainWindow(GameSettings::GetInstance().GetLong("Screen Width"), GameSettings::GetInstance().GetLong("Screen Height"), GameSettings::GetInstance().GetBool("Fullscreen"), GameSettings::GetInstance().GetBool("Borderless"), "Amesite", GameSettings::GetInstance().GetBool("Debug Context"));
    glewInit();
    window->Init(GameSettings::GetInstance().GetBool("Show Notifications"));
    window->SetVsync(GameSettings::GetInstance().GetBool("VSync"));
    Input()->SetAimDeadzone(GameSettings::GetInstance().GetDouble("Aim Deadzone"));
    Input()->SetMoveDeadzone(GameSettings::GetInstance().GetDouble("Move Deadzone"));
    Input()->AssignButton(InputHandler::PLAYER_ONE, InputHandler::PROFILE, InputHandler::KEYBOARD, GLFW_KEY_F2);
    
    System::SoundSystem* soundSystem = new System::SoundSystem();
    
    if (GameSettings::GetInstance().GetBool("Show Splash Screen"))
        Game::GetInstance().SetScene(new SplashScene());
    else
        Game::GetInstance().SetScene(new LoadingScene());
    
    Profiling().Init();
    bool profiling = false;
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        if (Input()->Triggered(InputHandler::ANYONE, InputHandler::PROFILE))
            profiling = !profiling;
        Profiling().SetActive(profiling);
        
        if (profiling)
            Profiling().BeginFrame();
        
        { PROFILE_CPU("Frame");
            // Update scene.
            { PROFILE_CPU("Update");
                window->Update();
                Game::GetInstance().Update(static_cast<float>(deltaTime));
            }
            
            // Wait for GPU to finish.
            { PROFILE_CPU("GPU Finish");
                glFinish();
            }
        }
        
        if (profiling) {
            Profiling().DrawResults();
            Profiling().EndFrame();
        }
        
        // Set window title to reflect screen update and render times.
        float frameTime = (glfwGetTime() - lastTime) * 1000.0f;
        std::string title = "Amesite";
        if (GameSettings::GetInstance().GetBool("Show Frame Times"))
            title += " - " + std::to_string(frameTime) + " ms";
        window->SetTitle(title.c_str());
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        long wait = static_cast<long>((1.0 / GameSettings::GetInstance().GetLong("Target FPS") + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        // Get input
        glfwPollEvents();
    }
    
    Profiling().Free();
    Game::GetInstance().Free();
    delete soundSystem;
    delete window;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    
    //_CrtDumpMemoryLeaks();
    return 0;
}
