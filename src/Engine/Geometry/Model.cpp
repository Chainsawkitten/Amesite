#include "Model.hpp"
#include "../Util/Log.hpp"

namespace Geometry {
    Model::Model(const char* filename) {
        std::ifstream modelFile;
        modelFile.open(filename, std::ios_base::binary);
        if (!modelFile.is_open())
            Log() << "Couldn't open model file " << filename << " for reading.\n";
        
        modelFile.read(reinterpret_cast<char*>(&vertexNr), sizeof(vertexNr));
        vertexData = new Vertex[vertexNr];
        modelFile.read(reinterpret_cast<char*>(&vertexData[0]), vertexNr * sizeof(Vertex));
        
        modelFile.close();
        
        indexNr = vertexNr;
        indexData = new unsigned int[indexNr];
        for (unsigned int i = 0; i < indexNr; i++) {
            indexData[i] = i;
        }
        
        GenerateBuffers();
        GenerateVertexArray();
    }
    
    Model::~Model() {
        delete[] vertexData;
        delete[] indexData;
    }
    
    Geometry::Geometry3D:: Vertex* Model::GetVertices() const {
        return vertexData;
    }
    
    unsigned int Model::GetVertexCount() const {
        return vertexNr;
    }
    
    unsigned int* Model::GetIndices() const {
        return indexData;
    }
    
    unsigned int Model::GetIndexCount() const {
        return indexNr;
    }
    
    void Model::Save(const char* filename) const {
        std::ofstream modelFile;
        modelFile.open(filename, std::ios_base::binary);
        
        modelFile.write(reinterpret_cast<const char*>(&vertexNr), sizeof(vertexNr));
        modelFile.write(reinterpret_cast<const char*>(&vertexData[0]), vertexNr * sizeof(Vertex));
        
        modelFile.close();
    }
}
