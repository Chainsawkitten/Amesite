#include "CollisionSystem.hpp"

#include "../Entity/Entity.hpp"
#include "../Scene/Scene.hpp"

#include "../Component/Collider2DCircle.hpp"
//#include "../Component/Collider2DRectangle.hpp"
#include "../Component/Transform.hpp"

#include <glm/glm.hpp>
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
    // Get components from scene.
    std::vector<Scene::Collision*>* collisions = scene.GetVector<Scene::Collision>();
    std::vector<Component::Collider2DCircle*> collider2DCircle = scene.GetAll<Component::Collider2DCircle>();

    // Clear collisions.
    for (Scene::Collision* collision : *collisions) {
        delete collision;
    }
    collisions->clear();

    Node node;
    glm::vec3 pos;
    glm::vec3 scale;
    std::vector<Node> nodes;
    for (auto collider : collider2DCircle) {
        if (collider->IsKilled())
            continue;

        node.collider = collider;
        Component::Transform* colliderTransform = collider->entity->GetComponent<Component::Transform>();
        pos = colliderTransform->GetWorldPosition();
        scale = colliderTransform->GetWorldScale();
        node.min.x = pos.x - collider->radius * scale.x;
        node.max.x = pos.x + collider->radius * scale.x;
        node.min.y = pos.z - collider->radius * scale.z;
        node.max.y = pos.z + collider->radius * scale.z;
        nodes.push_back(node);
    }

    std::sort(nodes.begin(), nodes.end(), myfunction);

    if (nodes.size() == 0)
        return;

    for (unsigned int x = 0; x < nodes.size() - 1; x++) {
        Scene::Collision* collisionX = nullptr;
        for (unsigned int i = x + 1; i < nodes.size() && nodes[x].max.x > nodes[i].min.x; i++) {
            Node* a;
            Node* b;
            if (nodes[i].max.x < nodes[x].max.x) {
                b = &nodes[i];
                a = &nodes[x];
            }
            else {
                a = &nodes[i];
                b = &nodes[x];
            }
            if (b->min.y < a->max.y && a->min.y < b->max.y) {
                // Circle vs Circle
                Component::Transform* aTransform = a->collider->entity->GetComponent<Component::Transform>();
                Component::Transform* bTransform = b->collider->entity->GetComponent<Component::Transform>();
                glm::vec3 aWPos = aTransform->GetWorldPosition();
                glm::vec3 bWPos = bTransform->GetWorldPosition();
                glm::vec3 aScale = aTransform->GetWorldScale();
                glm::vec3 bScale = bTransform->GetWorldScale();
                if (glm::length(aWPos - bWPos) < a->collider->radius * aScale.x + b->collider->radius * bScale.x) {
                    // x and y intersect each other.
                    if (collisionX == nullptr) {
                        collisionX = new Scene::Collision();
                        collisionX->entity = nodes[x].collider->entity;
                        collisions->push_back(collisionX);
                    }

                    // Check if collisionY is in mCollisonVec
                    Scene::Collision* collisionY = nullptr;
                    for (Scene::Collision* collision : *collisions)
                        if (collision->entity == nodes[i].collider->entity)
                            collisionY = collision;

                    // If collisionY isn't in vector;
                    if (collisionY == nullptr) {
                        collisionY = new Scene::Collision();
                        collisionY->entity = nodes[i].collider->entity;
                        collisions->push_back(collisionY);
                    }
                    collisionX->intersect.push_back(nodes[i].collider->entity);
                    collisionY->intersect.push_back(nodes[x].collider->entity);
                }
            }
        }
    }
}