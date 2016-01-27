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

    Entity cubeDefenderOfThePolyverseEntity;
    cubeDefenderOfThePolyverseEntity.CreateTransform();
    cubeDefenderOfThePolyverseEntity.CreateMesh();
    cubeDefenderOfThePolyverseEntity.mMesh->mGeometry = Resources().CreateCube();
    
    Shader* vertShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    ShaderProgram* shaderProgram = Resources().CreateShaderProgram( {vertShader, fragShader} );
    
    Entity testCamera;
    testCamera.CreateLens();
    testCamera.CreateTransform();

    testCamera.mTransform->Move(0.f, 0.0f, 5.f);
    testCamera.mTransform->Rotate(-15.f,0.f,0.f);

    shaderProgram->Use();

    glBindVertexArray(cubeDefenderOfThePolyverseEntity.mMesh->mGeometry->GetVertexArray());
    
    // Main game loop.
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        lastTime = glfwGetTime();

        testCamera.mTransform->Move(0.1f,0.f,0.f);

        glm::mat4 model = glm::translate(glm::mat4(), cubeDefenderOfThePolyverseEntity.mTransform->mPosition) * cubeDefenderOfThePolyverseEntity.mTransform->GetOrientation() * glm::scale(glm::mat4(), cubeDefenderOfThePolyverseEntity.mTransform->mScale);
        glm::mat4 view = testCamera.mTransform->GetOrientation()*glm::translate(glm::mat4(), -testCamera.mTransform->mPosition);
        glm::mat4 projection = testCamera.mLens->GetProjection(glm::vec2(800, 600));

        glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projection[0][0]);

        // Render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, cubeDefenderOfThePolyverseEntity.mMesh->mGeometry->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
        
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
