#include "Terrain.hpp"
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include <stb_image.h>

namespace Geometry {
    Terrain::Terrain(const char* filename) {
        // Load height map from file
        int components;
        unsigned char* data;
        data = stbi_load(filename, &width, &height, &components, 0);
    
        if (data == NULL)
            Log() << "Couldn't load image: " << filename << "\n";
    
        // Convert height map to float.
        heightMap = new float*[width];
        normals = new glm::vec3*[width];
        tangents = new glm::vec3*[width];
        for (int i = 0; i < width; i++) {
            heightMap[i] = new float[height];
            normals[i] = new glm::vec3[height];
            tangents[i] = new glm::vec3[height];
        }
    
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                heightMap[x][y] = data[(x + y*width)*components] / 256.f;
            }
        }
    
        stbi_image_free(data);
    
        Filter3x3();
        CalculateNormals();
    
        GenerateVertices();
        GenerateIndices();
    
        for (int i = 0; i < width; i++) {
            delete[] normals[i];
            delete[] tangents[i];
        }
        delete[] normals;
        delete[] tangents;
    
        GenerateBuffers();
        GenerateVertexArray();
    }

	Terrain::Terrain(float** floatArray, int width, int height, glm::vec2 textureRepeat ) {
		// Load height map from file
		this->width = width;
		this->height = height;

		heightMap = new float*[width];
		normals = new glm::vec3*[width];
		tangents = new glm::vec3*[width];
		for (int i = 0; i < width; i++) {
			heightMap[i] = new float[height];
			normals[i] = new glm::vec3[height];
			tangents[i] = new glm::vec3[height];
		}

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				heightMap[i][j] = floatArray[i][j];
			}
		}

		Filter3x3();
		CalculateNormals();

		GenerateVertices(textureRepeat);
		GenerateIndices();

		for (int i = 0; i < width; i++) {
			delete[] normals[i];
			delete[] tangents[i];
		}
		delete[] normals;
		delete[] tangents;

		GenerateBuffers();
		GenerateVertexArray();
	}
    
    Terrain::~Terrain() {
        delete[] vertexData;
        delete[] indexData;
        
        for (int i = 0; i < width; i++) {
            delete[] heightMap[i];
        }
        delete[] heightMap;
    }
    
    Geometry::Geometry3D::Vertex* Terrain::GetVertices() const {
        return vertexData;
    }
    
    unsigned int Terrain::GetVertexCount() const {
        return vertexNr;
    }
    
    unsigned int* Terrain::GetIndices() const {
        return indexData;
    }
    
    unsigned int Terrain::GetIndexCount() const {
        return indexNr;
    }
    
    float Terrain::GetY(float x, float z) const {
        float xInTerrain = x * width;
        float zInTerrain = z * height;
    
        if (xInTerrain < 0.f || xInTerrain >= width - 1 || zInTerrain < 0.f || zInTerrain >= height - 1) {
            return 0.f;
        }
    
        int xFloor = static_cast<int>(xInTerrain);
        int zFloor = static_cast<int>(zInTerrain);
    
        float dx = xInTerrain - xFloor;
        float dz = zInTerrain - zFloor;
    
        // Bilinear interpolation.
        return (1.f - dx)*(1.f - dz) * heightMap[xFloor][zFloor] +
               dx * (1.f - dz) * heightMap[xFloor + 1][zFloor] +
               (1.f - dx) * dz * heightMap[xFloor][zFloor + 1] +
               dx * dz * heightMap[xFloor + 1][zFloor + 1];
    }

    glm::vec3 Terrain::GetNormal(float x, float z) const {
		float xInTerrain = x * width;
		float zInTerrain = z * height;

		if (xInTerrain < 0.f || xInTerrain >= width - 1 || zInTerrain < 0.f || zInTerrain >= height - 1) {
			return glm::vec3(0.f,0.f,0.f);
		}
		int xFloor = static_cast<int>(xInTerrain);
		int zFloor = static_cast<int>(zInTerrain);
        
        // Get triangle.
		Vertex a, b, c;
		if (zInTerrain - zFloor > xInTerrain - xFloor){
            a = vertexData[xFloor + zFloor*width];
            b = vertexData[(xFloor + 1) + (zFloor + 1)*width];
            c = vertexData[(xFloor + 1) + zFloor*width];
		} else {
            a = vertexData[xFloor + zFloor*width];
            b = vertexData[xFloor + (zFloor + 1)*width];
            c = vertexData[(xFloor + 1) + (zFloor + 1)*width];
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
        return textureRepeat;
    }
    
    void Terrain::SetTextureRepeat(const glm::vec2& repeat) {
        textureRepeat = repeat;
    }
    
    void Terrain::GenerateVertices() {
        vertexNr = width * height;
        vertexData = new Vertex[vertexNr];
    
        for (unsigned int i = 0; i < vertexNr; i++) {
            vertexData[i] = {
                // Position
                glm::vec3(static_cast<float>(i % width) / width - 0.5f,
                          heightMap[i % width][i / width],
                          static_cast<float>(i / width) / height - 0.5f),
                // Texture coordinates
                glm::vec2(static_cast<float>(i % width) / width,
                          static_cast<float>(i / width) / height),
                // Normal
                glm::vec3(normals[i % width][i / width].x,
                          normals[i % width][i / width].y,
                          normals[i % width][i / width].z)
            };
        }
    }

    void Terrain::GenerateVertices(glm::vec2 textureRepeat) {
        vertexNr = width * height;
        vertexData = new Vertex[vertexNr];

        for (unsigned int i = 0; i < vertexNr; i++) {
            vertexData[i] = {
                // Position
                glm::vec3(static_cast<float>(i % width) / width - 0.5f,
                heightMap[i % width][i / width],
                    static_cast<float>(i / width) / height - 0.5f),
                // Texture coordinates
                glm::vec2(static_cast<float>(i % width) / width,
                    static_cast<float>(i / width) / height)*textureRepeat,
                // Normal
                glm::vec3(normals[i % width][i / width].x,
                    normals[i % width][i / width].y,
                    normals[i % width][i / width].z)
            };
        }
    }
    
    void Terrain::GenerateIndices() {
        indexNr = (width - 1) * (height - 1) * 6;
        indexData = new unsigned int[indexNr];
    
        for (unsigned int i = 0; i < indexNr; i += 6) {
            unsigned int x = (i / 6) % (width - 1);
            unsigned int y = (i / 6) / (width - 1);
    
            indexData[i] = x + y*width;
            indexData[i + 1] = (x + 1) + (y + 1)*width;
            indexData[i + 2] = (x + 1) + y*width;
            indexData[i + 3] = x + y*width;
            indexData[i + 4] = x + (y + 1)*width;
            indexData[i + 5] = (x + 1) + (y + 1)*width;
        }
    }
    
    void Terrain::Filter3x3() {
        float** filteredHeightMap = new float*[width];
        for (int i = 0; i < width; i++) {
            filteredHeightMap[i] = new float[height];
        }
    
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                filteredHeightMap[x][y] = SampleHeight(x, y);
            }
        }
    
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                heightMap[x][y] = filteredHeightMap[x][y];
            }
        }
    
        for (int i = 0; i < width; i++) {
            delete[] filteredHeightMap[i];
        }
        delete[] filteredHeightMap;
    }
    
    float Terrain::SampleHeight(int x, int y) const {
        int num = 0;
        float sum = 0.f;
    
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y; j++) {
                if (i >= 0 && i < width && j >= 0 && j < height) {
                    num++;
                    sum += heightMap[i][j];
                }
            }
        }
    
        return sum / num;
    }
    
    
    void Terrain::CalculateNormals() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                float sx = heightMap[x < width - 1 ? x + 1 : x][y] - heightMap[x > 0 ? x - 1 : x][y];
                if (x == 0 || x == width - 1)
                    sx *= 2.f;
    
                float sy = heightMap[x][y<height - 1 ? y + 1 : y] - heightMap[x][y > 0 ? y - 1 : y];
                if (y == 0 || y == height - 1)
                    sy *= 2.f;
    
                tangents[x][y] = glm::normalize(glm::vec3(2.f, sx, 0.f));
                normals[x][y] = glm::normalize(glm::vec3(-width * sx, 2.f, -height * sy));
            }
        }
    }
}
