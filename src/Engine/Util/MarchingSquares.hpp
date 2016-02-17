#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace MarchingSquares {
    /// The node that is used to create mesh.
    struct MeshNode {
        /// Position of node.
        glm::vec3 position;
        /// Index of node.
        int vertexIndex;
    };

    /// Node with data from the cellular automata.
    struct ControlNode {
        /// In mesh or not.
        bool active;
        /// Corresponding mesh nodes.
        MeshNode above, right;
    };

    /// Representation of a square in the algorithm
    struct MSquare {
        ControlNode topLeft, topRight, bottomRight, bottomLeft;
        MeshNode centerTop, centerRight, centerBottom, centerLeft;
        int type;
    };
    /// Perform the marching squares algorithm on a set of data.
    /**
    * @param data to analyze.
    * @return vector contraining the isolines. 
    */
    std::vector<glm::vec2> marchingSquares(const bool **data, const float squareSize);

    /// Create a marching square from a set of data.
    /**
     * @param points of the square.
     * @return type, index between 0-15.
     */
    MSquare createMSquare(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft);

    ControlNode createControlNode(const glm::vec3 position, const bool active, const float squareSize);

    MeshNode createMeshNode(const glm::vec3 position);



    /// Get the type of intersection in the square examined.
    /**
    * @param points of the square.
    * @return type, index between 0-15.
    */
    glm::vec2 getTriangle(MSquare square);
}