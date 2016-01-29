#include "Geometry2D.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

namespace Geometry {
    Geometry2D::~Geometry2D() {
        glDeleteBuffers(1, &mVertexBuffer);
        glDeleteBuffers(1, &mIndexBuffer);
        glDeleteVertexArrays(1, &mVertexArray);
    }
    
    GLuint Geometry2D::GetVertexArray() const {
        return mVertexArray;
    }
    
    void Geometry2D::GenerateBuffers() {
        // Vertex buffer
        glGenBuffers(1, &mVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, GetVertexCount() * sizeof(Vertex), GetVertices(), GL_STATIC_DRAW);
        
        // Index buffer
        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexCount() * sizeof(unsigned int), GetIndices(), GL_STATIC_DRAW);
    }
    
    void Geometry2D::GenerateVertexArray() {
        // Define vertex data layout
        glGenVertexArrays(1, &mVertexArray);
        glBindVertexArray(mVertexArray);
        
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));
        
        glBindVertexArray(0);
    }
}
