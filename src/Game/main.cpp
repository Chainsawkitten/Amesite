#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

using namespace std;

int main() {
    if (!glfwInit())
        return 1;
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Modership", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        /// @todo Print error to log.
    }
    glfwMakeContextCurrent(window);
    
    glewInit();
    
    glEnable(GL_DEPTH_TEST);
    
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
