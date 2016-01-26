#include "Cube.hpp"

namespace Geometry {
    Cube::Cube() {
        // Vertices
        mVertexNr = 24;
        mVertexData = new Vertex[mVertexNr];
    
        // Side 1
        mVertexData[0] = {
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        mVertexData[1] = {
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        mVertexData[2] = {
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        mVertexData[3] = {
            glm::vec3(-0.5f, 0.5f, 0.5f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
    
        // Side 2
        mVertexData[4] = {
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
        mVertexData[5] = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
        mVertexData[6] = {
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
        mVertexData[7] = {
            glm::vec3(-0.5f, 0.5f, 0.5f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
    
        // Side 3
        mVertexData[8] = {
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        mVertexData[9] = {
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        mVertexData[10] = {
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        mVertexData[11] = {
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
    
        // Side 4
        mVertexData[12] = {
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f)
        };
        mVertexData[13] = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f)
        };
        mVertexData[14] = {
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f)
        };
        mVertexData[15] = {
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f)
        };
    
        // Side 5
        mVertexData[16] = {
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        mVertexData[17] = {
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        mVertexData[18] = {
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        mVertexData[19] = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
    
        // Side 6
        mVertexData[20] = {
            glm::vec3(-0.5f, 0.5f, 0.5f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        mVertexData[21] = {
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        mVertexData[22] = {
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        mVertexData[23] = {
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
    
        // Vertexindices
        mIndexNr = 36;
        mIndexData = new unsigned int[mIndexNr];
    
        // Side 1
        mIndexData[0] = 0;
        mIndexData[1] = 1;
        mIndexData[2] = 2;
        mIndexData[3] = 0;
        mIndexData[4] = 3;
        mIndexData[5] = 1;
    
        // Side 2
        mIndexData[6] = 4;
        mIndexData[7] = 6;
        mIndexData[8] = 7;
        mIndexData[9] = 4;
        mIndexData[10] = 5;
        mIndexData[11] = 6;
    
        // Side 3
        mIndexData[12] = 11;
        mIndexData[13] = 9;
        mIndexData[14] = 8;
        mIndexData[15] = 11;
        mIndexData[16] = 10;
        mIndexData[17] = 9;
    
        // Side 4
        mIndexData[18] = 12;
        mIndexData[19] = 13;
        mIndexData[20] = 15;
        mIndexData[21] = 12;
        mIndexData[22] = 14;
        mIndexData[23] = 13;
    
        // Side 5
        mIndexData[24] = 16;
        mIndexData[25] = 19;
        mIndexData[26] = 18;
        mIndexData[27] = 16;
        mIndexData[28] = 18;
        mIndexData[29] = 17;
    
        // Side 6
        mIndexData[30] = 20;
        mIndexData[31] = 22;
        mIndexData[32] = 23;
        mIndexData[33] = 20;
        mIndexData[34] = 21;
        mIndexData[35] = 22;
    
        GenerateBuffers();
        GenerateVertexArray();
    }
    
    Cube::~Cube() {
        delete[] mVertexData;
        delete[] mIndexData;
    }
    
    Geometry3D::Vertex* Cube::GetVertices() const {
        return mVertexData;
    }
    
    unsigned int Cube::GetVertexCount() const {
        return mVertexNr;
    }
    
    unsigned int* Cube::GetIndices() const {
        return mIndexData;
    }
    
    unsigned int Cube::GetIndexCount() const {
        return mIndexNr;
    }
}
