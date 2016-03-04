#pragma once

#include "../Geometry/Geometry3D.hpp"

class Texture2D;

namespace Geometry {
    /// A terrain mesh.
    class Terrain : public Geometry3D {
        public:
            /// Create new terrain from the given height map.
            /**
             * @param filename Filename (relative or absolute) to height map texture.
             * @param textureRepeat How many times the texture should repeat across the terrain.
             */
            Terrain(const char* filename, const glm::vec2& textureRepeat);
            
            ///TODO comment.
            Terrain(float** floatArray, int width, int height, const glm::vec2& textureRepeat);
            
            /// Destructor
            ~Terrain();
            
            /// Get all the vertices.
            /**
             * @return Array of vertices
             */
            Vertex* GetVertices() const;
            
            /// Get the number of vertices.
            /**
             * @return The number of vertices
             */
            unsigned int GetVertexCount() const;
            
            /// Get all the vertex indices.
            /**
             * @return Array of vertex indices
             */
            unsigned int* GetIndices() const;
            
            /// Get the number of indicies.
            /**
             * @return The number of vertex indices.
             */
            unsigned int GetIndexCount() const;
            
            /// Gets the height of the terrain at a given location.
            /**
             * If the position is outside the terrain, 0.0 is returned.
             * @param x X-position on the terrain.
             * @param z Z-position on the terrain.
             * @return Height of the terrain (0.0 - 1.0).
             */
            float GetY(float x, float z) const;
            
            /// Gets the normal at given location
            /**
             * If the position is outside the terrain, 0.0, 0.0, 0.0 is returned.
             * @param x X-position in the world.
             * @param z Z-position in the world.
             * @return Normal at that point.
             */
            glm::vec3 GetNormal(float x, float z) const;
            
            /// Get how many times the texture should repeat.
            /**
             * Default: (1.0, 1.0)
             * @return How many times the texture should repeat
             */
            glm::vec2 GetTextureRepeat() const;
            
        private:
            void GenerateVertices();
            void GenerateIndices();
            void Filter3x3();
            float SampleHeight(int x, int y) const;
            void CalculateNormals();
            
            Vertex* mVertexData;
            unsigned int mVertexNr;
            
            unsigned int* mIndexData;
            unsigned int mIndexNr;
            
            float** mHeightMap;
            glm::vec3** mNormals;
            glm::vec3** mTangents;
            int mWidth;
            int mHeight;
            
            glm::vec2 mTextureRepeat;
    };
}
