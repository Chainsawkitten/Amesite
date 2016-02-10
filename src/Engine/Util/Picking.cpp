#include "Picking.hpp"
#include "../Util/Log.hpp"

glm::vec4 Picking::createWorldRay(const glm::vec2& mouseCoordinates, const glm::vec2& screenSize, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    //Get NDC x and y
    float vx = ((2.0f * mouseCoordinates.x) / screenSize.x) - 1.0f;
    float vy = 1.0f - ((2.0f * mouseCoordinates.y) / screenSize.y);

    glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
    glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);

    //Ray direction in clip space
    glm::vec4 rayClip(vx, vy, -1.0f, 0.0f);

    //Ray direction in view space
    glm::vec4 rayEye = inverseProjectionMatrix*rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    //Ray direction in world space
    glm::vec4 rayWorld = glm::vec4(inverseViewMatrix * rayEye);

    return glm::normalize(rayWorld);
}

glm::vec4 Picking::createPlayerAimDirection(const glm::vec4& worldRay, const glm::vec4& playerPosition, const glm::vec4& cameraPosition) {
    glm::vec4 cameraToPlane = (glm::vec4(0.f, playerPosition.y - cameraPosition.y, 0.f, 0.f));
    glm::vec4 pointInPlane = (cameraPosition + worldRay* (glm::length(cameraToPlane*cameraToPlane) / glm::length(worldRay*cameraToPlane) ) );
    return glm::normalize(glm::vec4(pointInPlane.x - playerPosition.x, 0.f, pointInPlane.z - playerPosition.z, 0.f));
}