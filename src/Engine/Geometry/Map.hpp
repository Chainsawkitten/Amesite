#pragma once

#include "Geometry3D.hpp"
#include <vector>
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
        * @return vector contraining the isolines.
        */
        Map(bool **data, const float squareSize, glm::uvec2 dataDimensions);

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

    private:
        /// The node that is used to create mesh.
        struct MeshNode {
                /// Position of node.
                glm::vec3 mPosition;
                /// Index of node.
                int mVertexIndex;
                /// Texture coordinates.
                glm::vec2 mTexCoords;
        };

        /// Node with data from the cellular automata.
        struct ControlNode : MeshNode{
                /// In mesh or not.
                bool mActive;
                /// Corresponding mesh nodes.
                MeshNode mAbove, mRight;
        };

        /// Representation of a square in the algorithm
        struct MSquare {
                ControlNode mTopLeft, mTopRight, mBottomRight, mBottomLeft;
                MeshNode mCenterTop, mCenterRight, mCenterBottom, mCenterLeft;
                int mType;
        };
        MeshNode createMeshNode(const glm::vec3 position, glm::uvec2 index, bool above, const float squareSize, glm::vec2 texCoords);

        ControlNode createControlNode(const glm::vec3 position, const bool active, const float squareSize, glm::uvec2 index);

        MSquare createMSquare(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft);

        void marchingSquares(bool **data, const float squareSize);

        void createMesh(MeshNode* position);

        void triangulateSquare(MSquare* square);

        void storeTriangle(MeshNode a, MeshNode b, MeshNode c);

        glm::uvec2 mDataDimensions;

        float mMapHeight, mMapWidth;

        std::vector<Vertex> mTempVertexData;
        std::vector<unsigned int> mTempIndexData;

        Vertex *mVertexData = nullptr;
        unsigned int mVertexNr = 0;

        unsigned int* mIndexData = nullptr;
        unsigned int mIndexNr = 0;
    };
}
