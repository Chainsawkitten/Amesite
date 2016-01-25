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
    
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
