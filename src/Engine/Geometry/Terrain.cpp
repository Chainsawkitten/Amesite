#include "Terrain.hpp"
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include <stb_image.h>
#include "../Texture/Texture2D.hpp"

namespace Geometry {
    Terrain::Terrain(const char* filename) {
        // Load height map from file
        int components;
        unsigned char* data;
        data = stbi_load(filename, &mWidth, &mHeight, &components, 0);
    
        if (data == NULL)
            Log() << "Couldn't load image: " << filename << "\n";
    
        // Convert height map to float.
        mHeightMap = new float*[mWidth];
        mNormals = new glm::vec3*[mWidth];
        mTangents = new glm::vec3*[mWidth];
        for (int i = 0; i < mWidth; i++) {
            mHeightMap[i] = new float[mHeight];
            mNormals[i] = new glm::vec3[mHeight];
            mTangents[i] = new glm::vec3[mHeight];
        }
    
        for (int x = 0; x < mWidth; x++) {
            for (int y = 0; y < mHeight; y++) {
                mHeightMap[x][y] = data[(x + y*mWidth)*components] / 256.f;
            }
        }
    
        stbi_image_free(data);
    
        Filter3x3();
        CalculateNormals();
    
        GenerateVertices();
        GenerateIndices();
    
        for (int i = 0; i < mWidth; i++) {
            delete[] mNormals[i];
            delete[] mTangents[i];
        }
        delete[] mNormals;
        delete[] mTangents;
    
        GenerateBuffers();
        GenerateVertexArray();
    }

    Terrain::Terrain(float** floatArray, int width, int height, glm::vec2 textureRepeat ) {
        // Load height map from file
        mWidth = width;
        mHeight = height;

        mHeightMap = new float*[mWidth];
        mNormals = new glm::vec3*[mWidth];
        mTangents = new glm::vec3*[mWidth];
        for (int i = 0; i < mWidth; i++) {
            mHeightMap[i] = new float[mHeight];
            mNormals[i] = new glm::vec3[mHeight];
            mTangents[i] = new glm::vec3[mHeight];
        }

        for (int i = 0; i < mWidth; i++) {
            for (int j = 0; j < mHeight; j++) {
                mHeightMap[i][j] = floatArray[i][j];
            }
        }

        Filter3x3();
        CalculateNormals();

        GenerateVertices(textureRepeat);
        GenerateIndices();

        for (int i = 0; i < mWidth; i++) {
            delete[] mNormals[i];
            delete[] mTangents[i];
        }
        delete[] mNormals;
        delete[] mTangents;

        GenerateBuffers();
        GenerateVertexArray();
    }
    
    Terrain::~Terrain() {
        delete[] mVertexData;
        delete[] mIndexData;
        
        for (int i = 0; i < mWidth; i++) {
            delete[] mHeightMap[i];
        }
        delete[] mHeightMap;
    }
    
    Geometry::Geometry3D::Vertex* Terrain::GetVertices() const {
        return mVertexData;
    }
    
    unsigned int Terrain::GetVertexCount() const {
        return mVertexNr;
    }
    
    unsigned int* Terrain::GetIndices() const {
        return mIndexData;
    }
    
    unsigned int Terrain::GetIndexCount() const {
        return mIndexNr;
    }
    
    float Terrain::GetY(float x, float z) const {
        float xInTerrain = x * mWidth;
        float zInTerrain = z * mHeight;
    
        if (xInTerrain < 0.f || xInTerrain >= mWidth - 1 || zInTerrain < 0.f || zInTerrain >= mHeight - 1) {
            return 0.f;
        }
    
        int xFloor = static_cast<int>(xInTerrain);
        int zFloor = static_cast<int>(zInTerrain);
    
        float dx = xInTerrain - xFloor;
        float dz = zInTerrain - zFloor;
    
        // Bilinear interpolation.
        return (1.f - dx)*(1.f - dz) * mHeightMap[xFloor][zFloor] +
               dx * (1.f - dz) * mHeightMap[xFloor + 1][zFloor] +
               (1.f - dx) * dz * mHeightMap[xFloor][zFloor + 1] +
               dx * dz * mHeightMap[xFloor + 1][zFloor + 1];
    }

    glm::vec3 Terrain::GetNormal(float x, float z) const {
        float xInTerrain = x * mWidth;
        float zInTerrain = z * mHeight;

        if (xInTerrain < 0.f || xInTerrain >= mWidth - 1 || zInTerrain < 0.f || zInTerrain >= mHeight - 1) {
            return glm::vec3(0.f, 0.f, 0.f);
        }
        int xFloor = static_cast<int>(xInTerrain);
        int zFloor = static_cast<int>(zInTerrain);
        
        // Get triangle.
        Vertex a, b, c;
        if (zInTerrain - zFloor > xInTerrain - xFloor){
            a = mVertexData[xFloor + zFloor*mWidth];
            b = mVertexData[(xFloor + 1) + (zFloor + 1)*mWidth];
            c = mVertexData[(xFloor + 1) + zFloor*mWidth];
        } else {
            a = mVertexData[xFloor + zFloor*mWidth];
            b = mVertexData[xFloor + (zFloor + 1)*mWidth];
            c = mVertexData[(xFloor + 1) + (zFloor + 1)*mWidth];
        }
        
        // Interpolate triangle normal.
        glm::vec3 pos(x, GetY(x, z), z);
        glm::vec3 edge1 = a.position - pos;
        glm::vec3 edge2 = b.position - pos;
        glm::vec3 edge3 = c.position - pos;
        
        // Calculate the areas and factors (order of parameters doesn't matter).
        float area = glm::length(glm::cross(a.position-b.position, a.position-c.position));
        float a1 = glm::length(glm::cross(edge2, edge3)) / area;
        float a2 = glm::length(glm::cross(edge3, edge1)) / area;
        float a3 = glm::length(glm::cross(edge1, edge2)) / area;
        
        return glm::normalize(a1 * a.normal + a2 * b.normal + a3 * c.normal);
    }
    
    glm::vec2 Terrain::GetTextureRepeat() const {
        return mTextureRepeat;
    }

    void Terrain::GenerateVertices(glm::vec2 textureRepeat) {
        mVertexNr = mWidth * mHeight;
        mVertexData = new Vertex[mVertexNr];

        for (unsigned int i = 0; i < mVertexNr; i++) {
            mVertexData[i] = {
                // Position
                glm::vec3(static_cast<float>(i % mWidth) / mWidth - 0.5f,
                          mHeightMap[i % mWidth][i / mWidth],
                          static_cast<float>(i / mWidth) / mHeight - 0.5f),
                // Texture coordinates
                glm::vec2(static_cast<float>(i % mWidth) / mWidth,
                          static_cast<float>(i / mWidth) / mHeight) * textureRepeat,
                // Normal
                glm::vec3(mNormals[i % mWidth][i / mWidth].x,
                          mNormals[i % mWidth][i / mWidth].y,
                          mNormals[i % mWidth][i / mWidth].z),
                // Tangent
                glm::vec3(mTangents[i % mWidth][i / mWidth].x,
                          mTangents[i % mWidth][i / mWidth].y,
                          mTangents[i % mWidth][i / mWidth].z)
            };
        }
    }
    
    void Terrain::GenerateIndices() {
        mIndexNr = (mWidth - 1) * (mHeight - 1) * 6;
        mIndexData = new unsigned int[mIndexNr];
    
        for (unsigned int i = 0; i < mIndexNr; i += 6) {
            unsigned int x = (i / 6) % (mWidth - 1);
            unsigned int y = (i / 6) / (mWidth - 1);
    
            mIndexData[i] = x + y*mWidth;
            mIndexData[i + 1] = (x + 1) + (y + 1)*mWidth;
            mIndexData[i + 2] = (x + 1) + y*mWidth;
            mIndexData[i + 3] = x + y*mWidth;
            mIndexData[i + 4] = x + (y + 1)*mWidth;
            mIndexData[i + 5] = (x + 1) + (y + 1)*mWidth;
        }
    }
    
    void Terrain::Filter3x3() {
        float** filteredHeightMap = new float*[mWidth];
        for (int i = 0; i < mWidth; i++) {
            filteredHeightMap[i] = new float[mHeight];
        }
    
        for (int x = 0; x < mWidth; x++) {
            for (int y = 0; y < mHeight; y++) {
                filteredHeightMap[x][y] = SampleHeight(x, y);
            }
        }
    
        for (int x = 0; x < mWidth; x++) {
            for (int y = 0; y < mHeight; y++) {
                mHeightMap[x][y] = filteredHeightMap[x][y];
            }
        }
    
        for (int i = 0; i < mWidth; i++) {
            delete[] filteredHeightMap[i];
        }
        delete[] filteredHeightMap;
    }
    
    float Terrain::SampleHeight(int x, int y) const {
        int num = 0;
        float sum = 0.f;
    
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y; j++) {
                if (i >= 0 && i < mWidth && j >= 0 && j < mHeight) {
                    num++;
                    sum += mHeightMap[i][j];
                }
            }
        }
    
        return sum / num;
    }
    
    
    void Terrain::CalculateNormals() {
        for (int x = 0; x < mWidth; x++) {
            for (int y = 0; y < mHeight; y++) {
                float sx = mHeightMap[x < mWidth - 1 ? x + 1 : x][y] - mHeightMap[x > 0 ? x - 1 : x][y];
                if (x == 0 || x == mWidth - 1)
                    sx *= 2.f;
    
                float sy = mHeightMap[x][y<mHeight - 1 ? y + 1 : y] - mHeightMap[x][y > 0 ? y - 1 : y];
                if (y == 0 || y == mHeight - 1)
                    sy *= 2.f;
    
                mTangents[x][y] = glm::normalize(glm::vec3(2.f, sx, 0.f));
                mNormals[x][y] = glm::normalize(glm::vec3(-mWidth * sx, 2.f, -mHeight * sy));
            }
        }
    }
}
