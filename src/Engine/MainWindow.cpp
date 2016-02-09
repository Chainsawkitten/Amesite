#include "Util/Log.hpp"
#include "MainWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

MainWindow* MainWindow::mInstance = nullptr;

MainWindow::MainWindow(int width, int height, bool fullscreen, bool borderless, const char* title, bool debugContext) {
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    if (borderless)
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    
    mDebugContext = debugContext;
    if (debugContext)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    
    mWindow = glfwCreateWindow(width, height, title, monitor, nullptr);
    if (!mWindow) {
        glfwTerminate();
        /// @todo Print error to log.
    }
    
    glfwMakeContextCurrent(mWindow);
    
    // Setup error callbacks.
    glfwSetErrorCallback(ErrorCallback);
    
    mInput = new InputHandler(mWindow);
    mInput->Update();
    mInput->SetActive();
    
    mSize = glm::vec2(width, height);
    mInstance = this;
}

MainWindow::~MainWindow() {
    glfwDestroyWindow(mWindow);
    delete mInput;
}

MainWindow* MainWindow::GetInstance() {
    return mInstance;
}

void MainWindow::Init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    if (mDebugContext)
        glDebugMessageCallback(DebugMessageCallback, nullptr);
}

void MainWindow::SetVsync(bool vsync) {
    glfwSwapInterval(vsync ? 1 : 0);
}

void MainWindow::SetControllerThresholds(double aimThreshold, double moveThreshold) {
    Input()->SetAimThreshold(aimThreshold);
    Input()->SetMoveThreshold(moveThreshold);
}

void MainWindow::Update() {
    mInput->Update();
    mInput->SetActive();
}

const glm::vec2& MainWindow::GetSize() const {
    return mSize;
}

void MainWindow::SetTitle(const char *title) {
    glfwSetWindowTitle(mWindow, title);
}

bool MainWindow::ShouldClose() const {
    return glfwWindowShouldClose(mWindow) != GL_FALSE;
}

void MainWindow::SwapBuffers() {
    glfwSwapBuffers(mWindow);
}
