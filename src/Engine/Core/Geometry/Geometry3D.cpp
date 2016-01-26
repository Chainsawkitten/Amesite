#include "Geometry3D.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

namespace Geometry {
    Geometry3D::~Geometry3D() {
        glDeleteBuffers(1, &mVertexBuffer);
        glDeleteBuffers(1, &mIndexBuffer);
    }
    
    
    GLuint Geometry3D::GetVertexArray() const {
        return mVertexArray;
    }
    
    void Geometry3D::GenerateBuffers() {
        // Vertex buffer
        glGenBuffers(1, &mVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, GetVertexCount() * sizeof(Vertex), GetVertices(), GL_STATIC_DRAW);
    
        // Index buffer
        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexCount() * sizeof(unsigned int), GetIndices(), GL_STATIC_DRAW);
    }
    
    void Geometry3D::GenerateVertexArray() {
        // Define vertex data layout
        glGenVertexArrays(1, &mVertexArray);
        glBindVertexArray(mVertexArray);
    
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry3D::Vertex), BUFFER_OFFSET(0));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry3D::Vertex), BUFFER_OFFSET(sizeof(float) * 3));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry3D::Vertex), BUFFER_OFFSET(sizeof(float) * 5));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry3D::Vertex), BUFFER_OFFSET(sizeof(float) * 8));
    
        glBindVertexArray(0);
    }
}
