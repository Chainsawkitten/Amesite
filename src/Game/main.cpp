#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include <Util/FileSystem.hpp>
#include <System/SoundSystem.hpp>

#include "Game.hpp"
#include "GameScenes/SplashScene.hpp"
#include "GameScenes/MenuScene.hpp"
#include "GameScenes/MainScene.hpp"
#include "GameScenes/WinScene.hpp"
//#include "GameScenes/PontusScene.hpp"
//#include "GameScenes/AlbinScene.hpp"
//#include "GameScenes/EmptyScene.hpp"
//#include "GameScenes/DanielScene.hpp"
#include "GameScenes/JonathanScene.hpp"
//#include "GameScenes/IvarScene.hpp"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <thread>
#include <vector>
#include <fstream>

using namespace std;

int main() {
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    //Enable logging if requested.
    if (GameSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::SavePath("Modership", "GameLog.txt").c_str(), "a", stderr);
    
    Log() << "Game started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    MainWindow* window = new MainWindow(GameSettings::GetInstance().GetLong("Screen Width"), GameSettings::GetInstance().GetLong("Screen Height"), GameSettings::GetInstance().GetBool("Fullscreen"), GameSettings::GetInstance().GetBool("Borderless"), "Modership", GameSettings::GetInstance().GetBool("Debug Context"));
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
        Game::GetInstance().SetScene(new JonathanScene());
    
    // Profiling variables.
    bool profiling = false;
    unsigned int profileFrames;
    float minFrameTime;
    float averageFrameTime;
    float maxFrameTime;

    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        window->Update();
        Game::GetInstance().Update(static_cast<float>(deltaTime));
        
        // Set window title to reflect screen update and render times.
        float frameTime = (glfwGetTime() - lastTime) * 1000.0f;
        std::string title = "Modership";
        if (GameSettings::GetInstance().GetBool("Show Frame Times"))
            title += " - " + std::to_string(frameTime) + " ms";
        window->SetTitle(title.c_str());
        
        // Profiling.
        if (profiling) {
            if (frameTime < minFrameTime)
                minFrameTime = frameTime;
            if (frameTime > maxFrameTime)
                maxFrameTime = frameTime;
            averageFrameTime += frameTime / 300.f;
            
            if (++profileFrames >= 300) {
                Log() << "Profiling ended - " << time(nullptr) << "\n";
                Log() << "Results:\n"
                      << "Min: " << minFrameTime << "\n"
                      << "Average: " << averageFrameTime << "\n"
                      << "Max: " << maxFrameTime << "\n";
                profiling = false;
            }
        } else if (Input()->Triggered(InputHandler::ANYONE, InputHandler::PROFILE)) {
            profiling = true;
            profileFrames = 0;
            minFrameTime = std::numeric_limits<float>::max();
            averageFrameTime = 0.f;
            maxFrameTime = 0.f;
            
            Log() << "Profiling started - " << time(nullptr) << "\n";
        }
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        long wait = static_cast<long>((1.0 / GameSettings::GetInstance().GetLong("Target FPS") + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        // Get input
        glfwPollEvents();
    }
    
    Game::GetInstance().Free();
    delete soundSystem;
    delete window;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";

    //_CrtDumpMemoryLeaks();
    return 0;
}
