#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <MainWindow.hpp>

#include <Engine/Geometry/Cube.hpp>
#include <Engine/Shader/Shader.hpp>
#include <Engine/Shader/ShaderProgram.hpp>

#include "Default3D.frag.hpp"
#include "Default3D.vert.hpp"

#include <Util/Log.hpp>
#include "Util/GameSettings.hpp"
#include <Util/FileSystem.hpp>

#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Lens.hpp>

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
    Component::Transform cubeTransform;
    
    Shader* vertShader = new Shader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragShader = new Shader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    ShaderProgram* shaderProgram = new ShaderProgram( {vertShader, fragShader} );
    
    Entity testCamera;
    testCamera.CreateLens();
    testCamera.CreateTransform();

    testCamera.mTransform->Move(2.f, 0.5f, 5.f);
    testCamera.mTransform->Rotate(-15.f,0.f,0.f);

    shaderProgram->Use();

    glm::mat4 model = glm::translate(glm::mat4(), cubeTransform.mPosition) * cubeTransform.GetOrientation() * glm::scale(glm::mat4(), cubeTransform.mScale);
    glm::mat4 view = testCamera.mTransform->GetOrientation()*glm::translate(glm::mat4(),-testCamera.mTransform->mPosition);
    glm::mat4 projection = testCamera.mLens->GetProjection(glm::vec2(800,600));

    glBindVertexArray(cubeDefenderOfThePolyverse.GetVertexArray());

    glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(shaderProgram->GetUniformLocation("view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(shaderProgram->GetUniformLocation("projection"), 1, GL_FALSE, &projection[0][0]);

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
