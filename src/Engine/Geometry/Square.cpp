#include "Square.hpp"

#include "Default2D.vert.hpp"
#include "SingleColor2D.frag.hpp"
#include "../Resources.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../MainWindow.hpp"

using namespace Geometry;

Square::Square() {
    // Vertices.
    mVertexNr = 4;
    mVertexData = new Vertex[mVertexNr];
    
    mVertexData[0] = {
        glm::vec2(-1.f, 1.f),
        glm::vec2(0.f, 1.f)
    };
    mVertexData[1] = {
        glm::vec2(1.f, 1.f),
        glm::vec2(1.f, 1.f)
    };
    mVertexData[2] = {
        glm::vec2(-1.f, -1.f),
        glm::vec2(0.f, 0.f)
    };
    mVertexData[3] = {
        glm::vec2(1.f, -1.f),
        glm::vec2(1.f, 0.f)
    };
    
    // Vertex indices.
    mIndexNr = 6;
    mIndexData = new unsigned int[mIndexNr];
    
    mIndexData[0] = 0;
    mIndexData[1] = 2;
    mIndexData[2] = 1;
    mIndexData[3] = 1;
    mIndexData[4] = 2;
    mIndexData[5] = 3;
    
    GenerateBuffers();
    GenerateVertexArray();
    
    mVertexShader = Resources().CreateShader(DEFAULT2D_VERT, DEFAULT2D_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(SINGLECOLOR2D_FRAG, SINGLECOLOR2D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
}

Square::~Square() {
    delete[] mVertexData;
    delete[] mIndexData;
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

Geometry2D::Vertex* Square::GetVertices() const {
    return mVertexData;
}

unsigned int Square::GetVertexCount() const {
    return mVertexNr;
}

unsigned int* Square::GetIndices() const {
    return mIndexData;
}

unsigned int Square::GetIndexCount() const {
    return mIndexNr;
}

void Square::Render(const glm::vec2 &position, const glm::vec2 &size, const glm::vec3 &color) const {
    // Disable depth testing.
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    // Draw background.
    mShaderProgram->Use();
    
    // Set color.
    glUniform3fv(mShaderProgram->GetUniformLocation("color"), 1, &color[0]);
    
    // Set location and size.
    glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();
    glUniform2fv(mShaderProgram->GetUniformLocation("position"), 1, &(position / screenSize)[0]);
    glUniform2fv(mShaderProgram->GetUniformLocation("size"), 1, &(size / screenSize)[0]);
    
    glBindVertexArray(GetVertexArray());
    
    glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    // Reset depth testing.
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
}
