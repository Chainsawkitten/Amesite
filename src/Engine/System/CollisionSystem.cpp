#include "CollisionSystem.hpp"

#include "../Entity/Entity.hpp"
#include "../Scene/Scene.hpp"

#include "../Component/Collider2DCircle.hpp"
//#include "../Component/Collider2DRectangle.hpp"
#include "../Component/Transform.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <algorithm>

using namespace System;
using namespace Component;

CollisionSystem::CollisionSystem() {
}

CollisionSystem::~CollisionSystem() {
}

struct Node {
    Component::Collider2DCircle* collider = nullptr;
    glm::vec2 min;
    glm::vec2 max;
};

bool myfunction(Node& i, Node& j) { return (i.min.x<j.min.x); }

void CollisionSystem::Update(Scene& scene) {
    // Get vectors from scene
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    std::vector<Component::Collider2DCircle*> collider2DCircle = scene.GetAll<Component::Collider2DCircle>();
    
    // Clear vector
    for (Scene::Collision* collision : *collisionVector) {
        delete collision;
    }
    collisionVector->clear();
 
    Node node;
    glm::vec3 pos;
    glm::vec3 scale;
    std::vector<Node> nodes;
    for (auto collider : collider2DCircle) {
        node.collider = collider;
        pos = collider->entity->GetComponent<Component::Transform>()->GetWorldPosition();
        scale = collider->entity->GetComponent<Component::Transform>()->GetWorldScale();
        node.min.x = pos.x - collider->radius * scale.x;
        node.max.x = pos.x + collider->radius * scale.x;
        node.min.y = pos.z - collider->radius * scale.z;
        node.max.y = pos.z + collider->radius * scale.z;
        nodes.push_back(node);
    }

    std::sort(nodes.begin(), nodes.end(), myfunction);

    for (unsigned int x = 0; x < nodes.size() - 1; x++) {
        Scene::Collision* collisionX = nullptr;
        for (unsigned int i = x + 1; i < nodes.size() && nodes[x].max.x > nodes[i].min.x; i++) {
            Node* a;
            Node* b;
            if (nodes[i].max.x < nodes[x].max.x) {
                b = &nodes[i];
                a = &nodes[x];
            } else {
                a = &nodes[i];
                b = &nodes[x];
            }
            if (b->min.y < a->max.y && a->min.y < b->max.y) {
                // Circle vs Circle
                glm::vec3 aWPos = a->collider->entity->GetComponent<Component::Transform>()->GetWorldPosition();
                glm::vec3 bWPos = b->collider->entity->GetComponent<Component::Transform>()->GetWorldPosition();
                glm::vec3 aScale = a->collider->entity->GetComponent<Component::Transform>()->GetWorldScale();
                glm::vec3 bScale = b->collider->entity->GetComponent<Component::Transform>()->GetWorldScale();
                if (glm::length(aWPos - bWPos) < a->collider->radius * aScale.x + b->collider->radius * bScale.x) {
                    // x and y intersect each other.
                    if (collisionX == nullptr) {
                        collisionX = new Scene::Collision();
                        collisionX->entity = nodes[x].collider->entity;
                        collisionVector->push_back(collisionX);
                    }

                    // check if collisionY is in mCollisonVec
                    Scene::Collision* collisionY = nullptr;
                    for (unsigned int m = 0; m < collisionVector->size() && collisionY == nullptr; m++)
                        if (collisionVector->at(m)->entity == nodes[i].collider->entity)
                            collisionY = collisionVector->at(m);

                    // if collisionY isn't in vector;
                    if (collisionY == nullptr) {
                        collisionY = new Scene::Collision();
                        collisionY->entity = nodes[i].collider->entity;
                        collisionVector->push_back(collisionY);
                    }
                    collisionX->intersect.push_back(nodes[i].collider->entity);
                    collisionY->intersect.push_back(nodes[x].collider->entity);
                }
            }
        }
    }

    /*for (unsigned int i = 0; i < mCollisionVec.size(); i++) {
        delete mCollisionVec.at(i);
    }
    mCollisionVec.clear();*/

    //// Rectangle vs Rectangle
    //for (unsigned int x = 0; x < scene.Size<Collider2DRectangle>(); x++) {
    //    Collider2DRectangle* colliderX = scene.Get<Collider2DRectangle>(x);
    //    Collision* collisionX = nullptr;
    //    // check if collisionX is in mCollisonVec
    //    for (unsigned int i = 0; i < mCollisionVec.size() && collisionX == nullptr; i++) {
    //        if (mCollisionVec.at(i)->entity == colliderX->entity)
    //            collisionX = mCollisionVec.at(i);
    //    }

    //    for (unsigned int y = x + 1; y < scene.Size<Collider2DRectangle>(); y++) {
    //        Collider2DRectangle* colliderY = scene.Get<Collider2DRectangle>(y);
    //        if (RectangleVSRectangle(colliderX, colliderY)) {
    //            // x and y intersect each other.
    //            if (collisionX == nullptr) {
    //                collisionX = new Collision();
    //                collisionX->entity = colliderX->entity;
    //                mCollisionVec.push_back(collisionX);
    //            }

    //            // check if collisionY is in mCollisonVec
    //            Collision* collisionY = nullptr;
    //            for (unsigned int i = 0; i < mCollisionVec.size() && collisionY == nullptr; i++) {
    //                if (mCollisionVec.at(i)->entity == colliderY->entity)
    //                    collisionY = mCollisionVec.at(i);
    //            }

    //            // if collisionY isn't in vector;
    //            if (collisionY == nullptr) {
    //                collisionY = new Collision();
    //                collisionY->entity = colliderY->entity;
    //                mCollisionVec.push_back(collisionY);
    //            }

    //            collisionX->intersect.push_back(colliderY->entity);
    //            collisionY->intersect.push_back(colliderX->entity);
    //        }
    //    }
    //}

    //// Rectangle vs Circle
    //for (unsigned int x = 0; x < scene.Size<Collider2DRectangle>(); x++) {
    //    Collider2DRectangle* colliderX = scene.Get<Collider2DRectangle>(x);
    //    Collision* collisionX = nullptr;
    //    // check if collisionX is in mCollisonVec
    //    for (unsigned int i = 0; i < mCollisionVec.size() && collisionX == nullptr; i++) {
    //        if (mCollisionVec.at(i)->entity == colliderX->entity)
    //            collisionX = mCollisionVec.at(i);
    //    }

    //    for (unsigned int y = 0; y < scene.Size<Collider2DCircle>(); y++) {
    //        Collider2DCircle* colliderY = scene.Get<Collider2DCircle>(y);
    //        if (RectangleVSCircle(colliderX, colliderY)) {
    //            // x and y intersect each other.
    //            if (collisionX == nullptr) {
    //                collisionX = new Collision();
    //                collisionX->entity = colliderX->entity;
    //                mCollisionVec.push_back(collisionX);
    //            }

    //            // check if collisionY is in mCollisonVec
    //            Collision* collisionY = nullptr;
    //            for (unsigned int i = 0; i < mCollisionVec.size() && collisionY == nullptr; i++) {
    //                if (mCollisionVec.at(i)->entity == colliderY->entity)
    //                    collisionY = mCollisionVec.at(i);
    //            }

    //            // if collisionY isn't in vector;
    //            if (collisionY == nullptr) {
    //                collisionY = new Collision();
    //                collisionY->entity = colliderY->entity;
    //                mCollisionVec.push_back(collisionY);
    //            }

    //            collisionX->intersect.push_back(colliderY->entity);
    //            collisionY->intersect.push_back(colliderX->entity);
    //        }
    //    }
    //}

    /*
    // Circle vs Circle
    for (unsigned int x = 0; x < collider2DCircle.size(); x++) {
        Collider2DCircle* colliderX = collider2DCircle.at(x);
        Scene::Collision* collisionX = nullptr;

        // check if collisionX is in mCollisonVec
        for (unsigned int i = 0; i < collisionVector->size() && collisionX == nullptr; i++)
            if (collisionVector->at(i)->entity == colliderX->entity)
                collisionX = collisionVector->at(i);

        for (unsigned int y = x + 1; y < collider2DCircle.size(); y++) {
            Collider2DCircle* colliderY = collider2DCircle.at(y);
            if (CircleVSCircle(colliderX, colliderY)) {
                // x and y intersect each other.
                if (collisionX == nullptr) {
                    collisionX = new Scene::Collision();
                    collisionX->entity = colliderX->entity;
                    collisionVector->push_back(collisionX);
                }

                // check if collisionY is in mCollisonVec
                Scene::Collision* collisionY = nullptr;
                for (unsigned int i = 0; i < collisionVector->size() && collisionY == nullptr; i++)
                    if (collisionVector->at(i)->entity == colliderY->entity)
                        collisionY = collisionVector->at(i);

                // if collisionY isn't in vector;
                if (collisionY == nullptr) {
                    collisionY = new Scene::Collision();
                    collisionY->entity = colliderY->entity;
                    collisionVector->push_back(collisionY);
                }

                collisionX->intersect.push_back(colliderY->entity);
                collisionY->intersect.push_back(colliderX->entity);
            }
        }
    }
    */
}


//bool CollisionSystem::RectangleVSRectangle(Collider2DRectangle* aRectangle, Collider2DRectangle* bRectangle) {
//    Transform* aTransform = aRectangle->entity->GetComponent<Transform>();
//    Transform* bTransform = bRectangle->entity->GetComponent<Transform>();
//    if (aTransform == nullptr)
//        return false;
//    if (bTransform == nullptr)
//        return false;
//    float rect1MinX, rect1MinZ, rect1MaxX, rect1MaxZ;
//    float rect2MinX, rect2MinZ, rect2MaxX, rect2MaxZ;
//    float dimX, dimZ;
//
//    glm::vec3 thisOrigin = aTransform->GetWorldPosition();
//    glm::vec3 otherOrigin = bTransform->GetWorldPosition();
//
//    // <<-------------------------- TODO SCALE ---------------------------------->> ugauoaghijafgioh suiogh sughy sbgyshiughsuiogh u<g ushpig hs8gh sugh uishgå 
//    glm::vec3 aScale = aTransform->GetWorldScale();
//    glm::vec3 bScale = bTransform->GetWorldScale();
//
//    dimX = aRectangle->width / 2 * aScale.x;
//    dimZ = aRectangle->height / 2 * bScale.z;
//
//    //Points we will use in order to create rectangles
//    rect1MaxX = thisOrigin.x + dimX;
//    rect1MaxZ = thisOrigin.z + dimZ; //Upper right corner
//
//    rect1MinX = thisOrigin.x - dimX;
//    rect1MinZ = thisOrigin.z - dimZ; //Lower left corner
//
//    dimX = bRectangle->width / 2 * bScale.x;
//    dimZ = bRectangle->height / 2 * bScale.y;
//
//    //Points we are going to test
//    rect2MaxX = otherOrigin.x + dimX;
//    rect2MaxZ = otherOrigin.z + dimZ; //Upper left corner
//
//    rect2MinX = otherOrigin.x - dimX;
//    rect2MinZ = otherOrigin.z - dimZ; //Lower right corner
//
//                                      //If both are true, there is no overlap
//    return  (!(rect1MinZ > rect2MaxZ || rect1MaxZ < rect2MinZ) && !(rect1MinX > rect2MaxX || rect1MaxX < rect2MinX));
//}
//
//bool CollisionSystem::RectangleVSCircle(Collider2DRectangle* rectangle, Collider2DCircle* circle) {
//    Transform* rectangleTransform = rectangle->entity->GetComponent<Transform>();
//    Transform* circleTransform = circle->entity->GetComponent<Transform>();
//    if (rectangleTransform == nullptr)
//        return false;
//    if (circleTransform == nullptr)
//        return false;
//
//    glm::vec3 rectangleOrigin = rectangleTransform->GetWorldPosition();
//    glm::vec3 circleOrigin = circleTransform->GetWorldPosition();
//
//    // <<-------------------------- TODO SCALE ---------------------------------->> ugauoaghijafgioh suiogh sughy sbgyshiughsuiogh u<g ushpig hs8gh sugh uishgå 
//
//    //Is the origin of the circle inside the rectangle?
//    if ((circleOrigin.x < (rectangleOrigin.x + (rectangle->width / 2))) &&
//        (circleOrigin.x > (rectangleOrigin.x - (rectangle->width / 2))) &&
//        (circleOrigin.z < (rectangleOrigin.z + (rectangle->height / 2))) &&
//        (circleOrigin.z > (rectangleOrigin.z - (rectangle->height / 2))))
//        return true;
//
//    glm::vec2 topLeft = glm::vec2(rectangleOrigin.x - (rectangle->width/2.f), rectangleOrigin.z + (rectangle->height / 2.f) );
//    glm::vec2 topRight = glm::vec2(rectangleOrigin.x + (rectangle->width / 2.f), rectangleOrigin.z + (rectangle->height / 2.f) );
//    glm::vec2 bottomLeft = glm::vec2(rectangleOrigin.x - (rectangle->width/2.f), rectangleOrigin.z - (rectangle->height / 2.f) );
//    glm::vec2 bottomRight = glm::vec2(rectangleOrigin.x + (rectangle->width / 2.f), rectangleOrigin.z - (rectangle->height / 2.f));
//
//    glm::vec2 circleCenter = glm::vec2(circleOrigin.x, circleOrigin.z);
//
//    //Is one of the vertices of the rectangle inside the circle?
//    if ( (glm::distance(circleCenter, topLeft) < circle->radius)        ||
//         (glm::distance(circleCenter, topRight) < circle->radius)       ||
//         (glm::distance(circleCenter, bottomRight) < circle->radius)    ||
//         (glm::distance(circleCenter, bottomLeft) < circle->radius) )
//        return true;
//
//    //Is the projection of the center of the circle onto one of the sides simultaneously on one of the sides and inside the circle?
//    //Tests assume axis aligned boxes
//    //Top & bottom side
//    if (circleCenter.x > topLeft.x && circleCenter.x < topRight.x) {
//        float distanceToSide = circleCenter.y - topRight.y;
//        if (distanceToSide < circle->radius && distanceToSide > 0.f)
//            return true;
//        
//        distanceToSide = bottomRight.y - circleCenter.y;
//        if (distanceToSide < circle->radius && distanceToSide > 0.f)
//            return true;
//    }
//
//    //Left & right side
//    if (circleCenter.y > bottomLeft.y && circleCenter.y < topLeft.y) {
//        float distanceToSide = topLeft.x - circleCenter.x;
//        if (distanceToSide < circle->radius && distanceToSide > 0.f)
//            return true;
//
//        distanceToSide = circleCenter.x - topRight.x;
//        if (distanceToSide < circle->radius && distanceToSide > 0.f)
//            return true;
//    }
//    return false;
//}
//
//bool CollisionSystem::CircleVSCircle(Collider2DCircle* aCircle, Collider2DCircle* bCircle) {
//    Transform* aTransform = aCircle->entity->GetComponent<Transform>();
//    Transform* bTransform = bCircle->entity->GetComponent<Transform>();
//    if (aTransform == nullptr)
//        return false;
//    if (bTransform == nullptr)
//        return false;
//
//    glm::vec3 circleAOrigin = aTransform->GetWorldPosition();
//    glm::vec3 circleBOrigin = bTransform->GetWorldPosition();
//    
//    glm::vec2 distance = glm::vec2(circleAOrigin.x, circleAOrigin.z) - glm::vec2(circleBOrigin.x, circleBOrigin.z);
//
//    if (glm::length(distance) < aCircle->radius * aTransform->GetWorldScale().x + bCircle->radius * bTransform->GetWorldScale().x)
//        return true;
//    return false;
//}
