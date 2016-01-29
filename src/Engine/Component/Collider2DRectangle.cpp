#include "Collider2DRectangle.hpp"
#include "Collider2DCircle.hpp"

using namespace Component;

Collider2DRectangle::Collider2DRectangle(float width, float height, Transform* mTransform) {
    this->width = width;
    this->height = height;
}

bool Collider2DRectangle::CollideRectangle(Entity* other) {
    //Check if other entity HAS the required components for testing
    if ( (other->GetComponent<Collider2DRectangle>() == nullptr) || (other->GetComponent<Transform>() == nullptr) )
        return false;
    float rect1MinX, rect1MinZ, rect1MaxX, rect1MaxZ;
    float rect2MinX, rect2MinZ, rect2MaxX, rect2MaxZ;
    float dimX, dimZ;
    glm::vec3 thisOrigin = parent->GetComponent<Transform>()->position;
    glm::vec3 otherOrigin = other->GetComponent<Transform>()->position;

    dimX = width / 2;
    dimZ = height / 2;

    //Points we will use in order to create triangles
    rect1MaxX = thisOrigin.x + dimX;
    rect1MaxZ = thisOrigin.z + dimZ; //Upper right corner

    rect1MinX = thisOrigin.x - dimX;
    rect1MinZ = thisOrigin.z - dimZ; //Lower left corner

    dimX = other->GetComponent<Collider2DRectangle>()->width / 2;
    dimZ = other->GetComponent<Collider2DRectangle>()->height / 2;

    //Points we are going to test
    rect2MaxX = otherOrigin.x + dimX;
    rect2MaxZ = otherOrigin.z + dimZ; //Upper left corner

    rect2MinX = otherOrigin.x - dimX;
    rect2MinZ = otherOrigin.z - dimZ; //Lower right corner

                                       //If both are true, there is no overlap
    return  (!(rect1MinZ > rect2MaxZ || rect1MaxZ < rect2MinZ) && !(rect1MinX > rect2MaxX || rect1MaxX < rect2MinX));
}

bool Collider2DRectangle::CollideCircle(Entity* other) {
    //Check if other entity HAS the required components for testing
    if ((other->GetComponent<Collider2DCircle>() == nullptr) || (other->GetComponent<Transform>() == nullptr))
        return false;
    //Is the origin of the circle inside the rectangle?
    if ((other->GetComponent<Transform>()->position.x < (parent->GetComponent<Transform>()->position.x + (parent->GetComponent<Collider2DRectangle>()->width / 2))) &&
        (other->GetComponent<Transform>()->position.x >(parent->GetComponent<Transform>()->position.x - (parent->GetComponent<Collider2DRectangle>()->width / 2))) &&
        (other->GetComponent<Transform>()->position.z < (parent->GetComponent<Transform>()->position.z + (parent->GetComponent<Collider2DRectangle>()->height / 2))) &&
        (other->GetComponent<Transform>()->position.z > (parent->GetComponent<Transform>()->position.z - (parent->GetComponent<Collider2DRectangle>()->height / 2))))
        return true;
    //Is one of the vertices of the rectangle inside the circle?
    glm::vec2 topLeft, topRight, bottomLeft, bottomRight;
    topLeft = glm::vec2(parent->GetComponent<Transform>()->position.x - (parent->GetComponent<Collider2DRectangle>()->width / 2), parent->GetComponent<Transform>()->position.z - (parent->GetComponent<Collider2DRectangle>()->height / 2));
    topRight = glm::vec2(parent->GetComponent<Transform>()->position.x + (parent->GetComponent<Collider2DRectangle>()->width / 2), parent->GetComponent<Transform>()->position.z - (parent->GetComponent<Collider2DRectangle>()->height / 2));
    bottomLeft = glm::vec2(parent->GetComponent<Transform>()->position.x - (parent->GetComponent<Collider2DRectangle>()->width / 2), parent->GetComponent<Transform>()->position.z + (parent->GetComponent<Collider2DRectangle>()->height / 2));
    bottomRight = glm::vec2(parent->GetComponent<Transform>()->position.x + (parent->GetComponent<Collider2DRectangle>()->width / 2), parent->GetComponent<Transform>()->position.z + (parent->GetComponent<Collider2DRectangle>()->height / 2));
    
    glm::vec2 circleCenter = glm::vec2(other->GetComponent<Transform>()->position.x, other->GetComponent<Transform>()->position.z);

    
    if ((glm::distance(circleCenter, topLeft) < other->GetComponent<Collider2DCircle>()->radius) ||
        (glm::distance(circleCenter, topRight) < other->GetComponent<Collider2DCircle>()->radius) ||
        (glm::distance(circleCenter, bottomRight) < other->GetComponent<Collider2DCircle>()->radius) ||
        (glm::distance(circleCenter, bottomLeft) < other->GetComponent<Collider2DCircle>()->radius)
        )
        return true;

    //Is the projection of the center of the circle onto one of the sides simultaneously on one of the sides and inside the circle?

}


Collider2DRectangle::~Collider2DRectangle() {

}