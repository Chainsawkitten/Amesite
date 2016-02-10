#pragma once

#include <glm/glm.hpp>

///Creates a ray in world space.
/**
 * @param mouseCoordinates The x/y position of the cursor.
 * @param screenSize The screens width/height
 * @param viewMatrix The view matrix
 */
glm::vec4 createWorldRay(const glm::vec2& mouseCoordinates, const glm::vec2& screenSize, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

glm::vec4 createPlayerAimDirection(const glm::vec4& worldRay, const glm::vec4& playerPosition, const glm::vec4& cameraPosition);