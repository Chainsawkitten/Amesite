#include "CollisionSystem.hpp"

#include "../Entity/Entity.hpp"

#include "../Component/Collider2DCircle.hpp"
#include "../Component/Collider2DRectangle.hpp"
#include "../Component/Transform.hpp"

#include <glm\glm.hpp>

using namespace System;
using namespace Component;

CollisionSystem::CollisionSystem() {
}

CollisionSystem::~CollisionSystem() {
}


bool CollisionSystem::RectangleVSRectangle(Entity *aRectangle, Entity *bRectangle) {
    if ((aRectangle->GetComponent<Collider2DRectangle>() == nullptr) || (aRectangle->GetComponent<Transform>() == nullptr))
        return false;
    if ((bRectangle->GetComponent<Collider2DRectangle>() == nullptr) || (bRectangle->GetComponent<Transform>() == nullptr))
        return false;
    float rect1MinX, rect1MinZ, rect1MaxX, rect1MaxZ;
    float rect2MinX, rect2MinZ, rect2MaxX, rect2MaxZ;
    float dimX, dimZ;

    glm::vec3 thisOrigin = aRectangle->GetComponent<Transform>()->position;
    glm::vec3 otherOrigin = bRectangle->GetComponent<Transform>()->position;
    Collider2DRectangle* RectangleA = aRectangle->GetComponent<Collider2DRectangle>();
    Collider2DRectangle* RectangleB = bRectangle->GetComponent<Collider2DRectangle>();;

    dimX = RectangleA->width / 2;
    dimZ = RectangleB->height / 2;

    //Points we will use in order to create triangles
    rect1MaxX = thisOrigin.x + dimX;
    rect1MaxZ = thisOrigin.z + dimZ; //Upper right corner

    rect1MinX = thisOrigin.x - dimX;
    rect1MinZ = thisOrigin.z - dimZ; //Lower left corner

    dimX = RectangleB->width / 2;
    dimZ = RectangleB->height / 2;

    //Points we are going to test
    rect2MaxX = otherOrigin.x + dimX;
    rect2MaxZ = otherOrigin.z + dimZ; //Upper left corner

    rect2MinX = otherOrigin.x - dimX;
    rect2MinZ = otherOrigin.z - dimZ; //Lower right corner

                                      //If both are true, there is no overlap
    return  (!(rect1MinZ > rect2MaxZ || rect1MaxZ < rect2MinZ) && !(rect1MinX > rect2MaxX || rect1MaxX < rect2MinX));
}

bool CollisionSystem::RectangleVSCircle(Entity *aRectangle, Entity *bCircle) {
    if ((aRectangle->GetComponent<Collider2DRectangle>() == nullptr || aRectangle->GetComponent<Transform>() == nullptr))
        return false;

    if ((bCircle->GetComponent<Collider2DCircle>() == nullptr) || (bCircle->GetComponent<Transform>() == nullptr))
        return false;

    Collider2DRectangle* rectangle = aRectangle->GetComponent<Collider2DRectangle>();
    glm::vec3 rectangleOrigin = aRectangle->GetComponent<Transform>()->position;

    Collider2DCircle* circle = bCircle->GetComponent<Collider2DCircle>();
    glm::vec3 circleOrigin = bCircle->GetComponent<Transform>()->position;

    //Is the origin of the circle inside the rectangle?
    if ((circleOrigin.x < (rectangleOrigin.x + (rectangle->width / 2))) &&
        (circleOrigin.x > (rectangleOrigin.x - (rectangle->width / 2))) &&
        (circleOrigin.z < (rectangleOrigin.z + (rectangle->height / 2))) &&
        (circleOrigin.z > (rectangleOrigin.z - (rectangle->height / 2))))
        return true;

    glm::vec2 topLeft = glm::vec2(rectangleOrigin.x - (rectangle->width/2.f), rectangleOrigin.z + (rectangle->height / 2.f) );
    glm::vec2 topRight = glm::vec2(rectangleOrigin.x + (rectangle->width / 2.f), rectangleOrigin.z + (rectangle->height / 2.f) );
    glm::vec2 bottomLeft = glm::vec2(rectangleOrigin.x - (rectangle->width/2.f), rectangleOrigin.z - (rectangle->height / 2.f) );
    glm::vec2 bottomRight = glm::vec2(rectangleOrigin.x + (rectangle->width / 2.f), rectangleOrigin.z - (rectangle->height / 2.f));

    glm::vec2 circleCenter = glm::vec2(circleOrigin.x, circleOrigin.z);

    //Is one of the vertices of the rectangle inside the circle?
    if ( (glm::distance(circleCenter, topLeft) < circle->radius)        ||
         (glm::distance(circleCenter, topRight) < circle->radius)       ||
         (glm::distance(circleCenter, bottomRight) < circle->radius)    ||
         (glm::distance(circleCenter, bottomLeft) < circle->radius) )
        return true;

    //Is the projection of the center of the circle onto one of the sides simultaneously on one of the sides and inside the circle?
    //Tests assume axis aligned boxes
    //Top & bottom side
    if (circleCenter.x > topLeft.x && circleCenter.x < topRight.x) {
        float distanceToSide = circleCenter.y - topRight.y;
        if (distanceToSide < circle->radius && distanceToSide > 0.f)
            return true;
        
        distanceToSide = bottomRight.y - circleCenter.y;
        if (distanceToSide < circle->radius && distanceToSide > 0.f)
            return true;
    }

    //Left & right side
    if (circleCenter.y > bottomLeft.y && circleCenter.y < topLeft.y) {
        float distanceToSide = topLeft.x - circleCenter.x;
        if (distanceToSide < circle->radius && distanceToSide > 0.f)
            return true;

        distanceToSide = circleCenter.x - topRight.x;
        if (distanceToSide < circle->radius && distanceToSide > 0.f)
            return true;
    }
    return false;
}

bool CollisionSystem::CircleVSCircle(Entity *aCircle, Entity *bCircle) {
    if ((aCircle->GetComponent<Collider2DCircle>() == nullptr || aCircle->GetComponent<Transform>() == nullptr))
        return false;

    if ((bCircle->GetComponent<Collider2DCircle>() == nullptr) || (bCircle->GetComponent<Transform>() == nullptr))
        return false;

    Collider2DCircle* circleA = aCircle->GetComponent<Collider2DCircle>();
    glm::vec2 circleAOrigin = glm::vec2(aCircle->GetComponent<Transform>()->position.x, aCircle->GetComponent<Transform>()->position.z);

    Collider2DCircle* circleB = bCircle->GetComponent<Collider2DCircle>();
    glm::vec2 circleBOrigin = glm::vec2(bCircle->GetComponent<Transform>()->position.x, bCircle->GetComponent<Transform>()->position.z);

    //Is the combined radius of the two circles smaller than the distance from the circles centers?
    if (glm::distance(circleAOrigin, circleBOrigin) < (circleA->radius+circleB->radius))
        return true;

    return false;
}
