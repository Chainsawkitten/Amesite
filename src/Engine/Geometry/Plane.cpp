#include "Plane.hpp"

using namespace Geometry;

Plane::Plane() {
    // Vertices
    mVertexNr = 4;
    mVertexData = new Vertex[mVertexNr];
    
    mVertexData[0] = {
        glm::vec3(-0.5f, 0.5f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };
    mVertexData[1] = {
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };
    mVertexData[2] = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };
    mVertexData[3] = {
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };
    
    // Vertexindices
    mIndexNr = 6;
    mIndexData = new unsigned int[mIndexNr];
    
    // Side 1
    mIndexData[0] = 0;
    mIndexData[1] = 2;
    mIndexData[2] = 1;
    mIndexData[3] = 1;
    mIndexData[4] = 2;
    mIndexData[5] = 3;
    
    GenerateBuffers();
    GenerateVertexArray();
}

Plane::~Plane() {
    delete[] mVertexData;
    delete[] mIndexData;
}

Geometry3D::Vertex* Plane::GetVertices() const {
    return mVertexData;
}

unsigned int Plane::GetVertexCount() const {
    return mVertexNr;
}

unsigned int* Plane::GetIndices() const {
    return mIndexData;
}

unsigned int Plane::GetIndexCount() const {
    return mIndexNr;
}
