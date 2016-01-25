#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>
#include <Util/Log.hpp>

using namespace std;

int main() {
    Log() << "Game started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    MainWindow* window = new MainWindow(800, 600, "Modership");
    glewInit();
    window->Init();
    
    while (!window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window->SwapBuffers();
        glfwPollEvents();
    }
    
    delete window;
    glfwTerminate();
    
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    
    return 0;
}
