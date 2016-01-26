#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MainWindow.hpp>

#include <Engine\Geometry\Cube.hpp>
#include <Engine\Shader\Shader.hpp>
#include <Engine\Shader\ShaderProgram.hpp>

#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include <Util/FileSystem.hpp>
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

    Geometry::Cube cubeDefenderOfThePolyverse;
    
    Shader* vertShader = new Shader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragShader = new Shader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    ShaderProgram* shaderProgram = new ShaderProgram( {vertShader, fragShader} );
    
    shaderProgram->Use();

    glBindVertexArray(cubeDefenderOfThePolyverse.GetVertexArray());

    while (!window->ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, cubeDefenderOfThePolyverse.GetIndexCount(), GL_UNSIGNED_INT, (void*)0);

        window->SwapBuffers();
        glfwPollEvents();
    }

    delete vertShader;
    delete fragShader;
    delete shaderProgram;
    
    delete window;

    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    return 0;
}
