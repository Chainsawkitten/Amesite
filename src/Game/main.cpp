#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>
#include <Engine\Core\Geometry\Cube.hpp>
#include <Engine\Core\Shader\Shader.hpp>
#include <Engine\Core\Shader\ShaderProgram.hpp>
#include <iostream>
#include <fstream>
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

using namespace std;

int main() {
    if (!glfwInit())
        return 1;
    
    MainWindow* window = new MainWindow(800, 600, "Modership");
    glewInit();
    window->Init();

    Geometry::Cube cubeDefenderOfThePolyverse;

    Shader* vertShader = new Shader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragShader = new Shader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    ShaderProgram* shaderProgram = new ShaderProgram( {vertShader, fragShader} );
    
    shaderProgram->Use();

    while (!window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, cubeDefenderOfThePolyverse.GetVertexArray(), cubeDefenderOfThePolyverse.GetVertexCount());

        window->SwapBuffers();
        glfwPollEvents();
    }
    
    delete window;
    glfwTerminate();
    
    return 0;
}
