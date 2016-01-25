#include "MainWindow.hpp"
#include <GLFW/glfw3.h>

MainWindow::MainWindow(int width, int height, bool fullscreen, const char* title) {
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    
    mWindow = glfwCreateWindow(width, height, title, monitor, nullptr);
    if (!mWindow) {
        glfwTerminate();
        /// @todo Print error to log.
    }
    
    glfwMakeContextCurrent(mWindow);
}

MainWindow::~MainWindow() {
    glfwDestroyWindow(mWindow);
}

void MainWindow::Init() {
    glEnable(GL_DEPTH_TEST);
}

bool MainWindow::ShouldClose() const {
    return glfwWindowShouldClose(mWindow) != GL_FALSE;
}

void MainWindow::SwapBuffers() {
    glfwSwapBuffers(mWindow);
}
