#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <MainWindow.hpp>

#include <Geometry/Cube.hpp>
#include <Shader/Shader.hpp>
#include <Shader/ShaderProgram.hpp>

#include <Resources.hpp>
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include "CaveSystem/CaveSystem.hpp"
#include <Util/FileSystem.hpp>

#include <Entity/Entity.hpp>

#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/Mesh.hpp>

#include <Resources.hpp>

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

    Caves::CaveSystem testCaveSystem;
    testCaveSystem.GenerateCaveSystem();

    Shader* vertShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    ShaderProgram* shaderProgram = Resources().CreateShaderProgram( {vertShader, fragShader} );
    
    Entity testCamera;
    testCamera.AddComponent<Component::Lens>();
    testCamera.AddComponent<Component::Transform>();


    testCamera.GetComponent<Component::Transform>()->Move(12.5f, -12.5f, 35.f);
    testCamera.GetComponent<Component::Transform>()->Rotate(0.f, 0.f, 0.f);

    shaderProgram->Use();
    
    
    
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    int numberOfWalls = testCaveSystem.walls.size();
    glBindVertexArray(testCaveSystem.walls[0]->GetComponent<Component::Mesh>()->geometry->GetVertexArray());
    while (!window->ShouldClose()) {
        lastTime = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < numberOfWalls; i++) {
            
        
            Component::Transform* cubeTransform = testCaveSystem.walls[i]->GetComponent<Component::Transform>();
            glm::mat4 model = glm::translate(glm::mat4(), cubeTransform->position) * cubeTransform->GetOrientation() * glm::scale(glm::mat4(), cubeTransform->scale);
            glm::mat4 view = testCamera.GetComponent<Component::Transform>()->GetOrientation() * glm::translate(glm::mat4(), -testCamera.GetComponent<Component::Transform>()->position);
            glm::mat4 projection = testCamera.GetComponent<Component::Lens>()->GetProjection(glm::vec2(800, 600));
        
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projection[0][0]);
        
            // Render.        
            glDrawElements(GL_TRIANGLES, testCaveSystem.walls[i]->GetComponent<Component::Mesh>()->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
        }

        // Set window title to reflect screen update and render times.
        std::string title = "Modership";
        if (GameSettings::GetInstance().GetBool("Show Frame Times"))
            title += " - " + std::to_string((glfwGetTime() - lastTime) * 1000.0) + " ms";
        window->SetTitle(title.c_str());
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        long wait = static_cast<long>((1.0 / GameSettings::GetInstance().GetLong("Target FPS") + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        glfwPollEvents();
    }
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertShader);
    Resources().FreeShader(fragShader);
    
    Resources().FreeCube();
    
    delete window;
    
    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";
    return 0;
}
