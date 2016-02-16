#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace MarchingSquares {
    /// Perform the marching squares algorith on a set of data.
    /**
    * @param data to analyze.
    * @return vector contraining the isolines. 
    */
    std::vector<glm::vec2> marchingSquares(const bool **data);

    /// Get the type of intersection in the square examined.
    /**
     * @param points of the square.
     * @return type, index between 0-15.
     */
    int getType(const glm::bvec4& points);

    /// Get the type of intersection in the square examined.
    /**
    * @param points of the square.
    * @return type, index between 0-15.
    */
    glm::vec2 getLine(const glm::vec2 points[4], const int type);
}