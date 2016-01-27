#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <MainWindow.hpp>

#include <Engine/Geometry/Cube.hpp>
#include <Engine/Shader/Shader.hpp>
#include <Engine/Shader/ShaderProgram.hpp>

#include <Resources.hpp>
#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include <Util/FileSystem.hpp>

#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Mesh.hpp>

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

    const int nrOfCubes = 10;
    Entity caveSystem[nrOfCubes];
    caveSystem[0].AddComponent<Component::Mesh>();
    caveSystem[0].AddComponent<Component::Transform>();
    caveSystem[0].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, 4.f));
    caveSystem[0].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[1].AddComponent<Component::Mesh>();
    caveSystem[1].AddComponent<Component::Transform>();
    caveSystem[1].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, 3.f));
    caveSystem[1].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[2].AddComponent<Component::Mesh>();
    caveSystem[2].AddComponent<Component::Transform>();
    caveSystem[2].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, 2.f));
    caveSystem[2].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[2].AddComponent<Component::Mesh>();
    caveSystem[2].AddComponent<Component::Transform>();
    caveSystem[2].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, 1.f));
    caveSystem[2].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[3].AddComponent<Component::Mesh>();
    caveSystem[3].AddComponent<Component::Transform>();
    caveSystem[3].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, 0.f));
    caveSystem[3].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[4].AddComponent<Component::Mesh>();
    caveSystem[4].AddComponent<Component::Transform>();
    caveSystem[4].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, -1.f));
    caveSystem[4].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[5].AddComponent<Component::Mesh>();
    caveSystem[5].AddComponent<Component::Transform>();
    caveSystem[5].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, -2.f));
    caveSystem[5].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[6].AddComponent<Component::Mesh>();
    caveSystem[6].AddComponent<Component::Transform>();
    caveSystem[6].GetComponent<Component::Transform>()->Move(glm::vec3(4.f, 0.f, -3.f));
    caveSystem[6].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[7].AddComponent<Component::Mesh>();
    caveSystem[7].AddComponent<Component::Transform>();
    caveSystem[7].GetComponent<Component::Transform>()->Move(glm::vec3(3.f, 0.f, -3.f));
    caveSystem[7].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[8].AddComponent<Component::Mesh>();
    caveSystem[8].AddComponent<Component::Transform>();
    caveSystem[8].GetComponent<Component::Transform>()->Move(glm::vec3(2.f, 0.f, -3.f));
    caveSystem[8].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();

    caveSystem[9].AddComponent<Component::Mesh>();
    caveSystem[9].AddComponent<Component::Transform>();
    caveSystem[9].GetComponent<Component::Transform>()->Move(glm::vec3(1.f, 0.f, -3.f));
    caveSystem[9].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    
    Shader* vertShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    ShaderProgram* shaderProgram = Resources().CreateShaderProgram( {vertShader, fragShader} );
    
    Entity testCamera;
    testCamera.AddComponent<Component::Lens>();
    testCamera.AddComponent<Component::Transform>();


    testCamera.GetComponent<Component::Transform>()->Move(0.f, 15.0f, 0.f);
    testCamera.GetComponent<Component::Transform>()->Rotate(0.f, 90.f, 0.f);

    shaderProgram->Use();
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        lastTime = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < nrOfCubes; i++){
            glBindVertexArray(caveSystem[i].GetComponent<Component::Mesh>()->geometry->GetVertexArray());

            glm::mat4 model = glm::translate(glm::mat4(), caveSystem[i].GetComponent<Component::Transform>()->mPosition) * caveSystem[i].GetComponent<Component::Transform>()->GetOrientation() * glm::scale(glm::mat4(), caveSystem[i].GetComponent<Component::Transform>()->mScale);
            glm::mat4 view = testCamera.GetComponent<Component::Transform>()->GetOrientation()*glm::translate(glm::mat4(), -testCamera.GetComponent<Component::Transform>()->mPosition);
            glm::mat4 projection = testCamera.GetComponent<Component::Lens>()->GetProjection(glm::vec2(800, 600));

            glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projection[0][0]);
        
            glDrawElements(GL_TRIANGLES, caveSystem[i].GetComponent<Component::Mesh>()->geometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
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
