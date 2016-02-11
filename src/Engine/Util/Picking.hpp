#pragma once

#include <glm/glm.hpp>

namespace Picking {
    ///Creates a ray in world space.
    /**
     * @param mouseCoordinates The x/y position of the cursor.
     * @param viewMatrix The view matrix.
     * @param projectionMatrix The projection matrix.
     * @return Ray direction in world space.
     */
    glm::vec4 createWorldRay(const glm::vec2& mouseCoordinates, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    ///Creates a vector representing the players aim direction
    /**
     *@param worldRay Ray direction in world space.
     *@param playerPosition Player position in world space.
     *@param cameraPosition Camera position in world space.
     *@return Vector from player to mouse aimpoint in world space.
     */
    glm::vec4 createPlayerAimDirection(const glm::vec4& worldRay, const glm::vec4& playerPosition, const glm::vec4& cameraPosition);
}