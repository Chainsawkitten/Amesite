#pragma once

#include "Geometry3D.hpp"
#include <vector>
#include <map>
#include <unordered_set>
#include <fstream>

namespace Geometry {
    /// A mesh loaded from marching square algorithm.
    class Map : public Geometry3D {
    public:
        /// Create a map mesh from a set of data.
        /**
        * @param data to analyze.
        * @param squareSize of a square in the grid.
        * @param dataDimensions of array of data.
        * @param wallHeight sets the height of wall mesh.
        * @return vector contraining the isolines.
        */
        Map(bool **data, glm::uvec2 dataDimensions, float wallHeight);

        /// Destructor
        ~Map();

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

        /// Get the type map
        /**
         * @return the two dimensional map containing information about marching cubes.
         */
        int** GetTypeMap() const;

        /// Get amount of times texture should repeat
        /**
         * @return number of times the texture repeats in x and y.
         */
        glm::vec2 TextureRepeat() const;

        /// Set texture repeat.
        /**
         * @param repeat the amount of times to repeat in x and y.
         */
        void SetTextureRepeat(const glm::vec2 repeat);
    private:
        // The node that is used to create mesh.
        struct MeshNode {
            // Position of node.
            glm::vec3 mPosition;
            // Index of node.
            int mVertexIndex;
            // Texture coordinates.
            glm::vec2 mTexCoords;
        };

        // Node with data from the cellular automata.
        struct ControlNode : MeshNode{
            // In mesh or not.
            bool mActive;
            // Corresponding mesh nodes.
            MeshNode mAbove, mRight;
        };

        // Representation of a square in the algorithm.
        struct MSquare {
            ControlNode mTopLeft, mTopRight, mBottomRight, mBottomLeft;
            MeshNode mCenterTop, mCenterRight, mCenterBottom, mCenterLeft;
            int mType;
        };

        // Triangle representation used when generating walls.
        struct MapTriangle {
            int indexA;
            int indexB;
            int indexC;

            int indices[3];
            bool Contains(int index);
        };

        // Node creation.
        MeshNode CreateMeshNode(const glm::vec3 position, glm::uvec2 index, bool above, const float squareSize, glm::vec2 texCoords);
        ControlNode CreateControlNode(const glm::vec3 position, const bool active, const float squareSize, glm::uvec2 index);
        MSquare CreateMSquare(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft);
        MapTriangle CreateMapTriangle(int a, int b, int c);

        // Computations for retrieving top mesh.
        void MarchingSquares(bool **data, const float squareSize);
        void CreateMesh(MeshNode* position, unsigned int size);

        // Computations for retrieving wall mesh.
        void CreateWallMesh();
        void CalculateMeshOutlines();
        bool IsOutline(int vertexA, int vertexB);
        int GetConnectedVertex(int index);
        void AddToDictionary(int indexKey, MapTriangle triangle);
        void FollowOutline(int index, int outlineIndex);

        // Creating and storing triangles.
        void TriangulateSquare(MSquare* square);
        void StoreTriangle(MeshNode a, MeshNode b, MeshNode c);

        // Data
        glm::uvec2 mDataDimensions;
        float mMapHeight, mMapWidth, mWallHeight;
        std::unordered_set<int> mVertexChecked;
        glm::vec2 mTextureRepeat;

        // List of all outlines.
        std::vector<std::vector<int>> mOutlines;

        // Mapping indices to the triangles that contain them.
        std::map<int, std::vector<MapTriangle>> mTriangleDictionary;

        // Mapping vertices solely based on position - to assist creating outlines.
        std::map<std::vector<int>, int> mVertexIndexMap;

        // Vertex and index data
        std::vector<Vertex> mTempVertexData;
        std::vector<unsigned int> mTempIndexData;

        Vertex *mVertexData = nullptr;
        unsigned int mVertexNr = 0;

        unsigned int* mIndexData = nullptr;
        unsigned int mIndexNr = 0;

        int** mTypeMap;

    };
}
