#include "Physics.hpp"

using namespace Component;

Physics::Physics(Entity* entity) : SuperComponent(entity) {
    velocity = glm::vec3(0.f, 0.f, 0.f);
    maxVelocity = 20.f;
    angularVelocity = glm::vec3(0.f, 0.f, 0.f);
    maxAngularVelocity = 2.f;
    acceleration = glm::vec3(0.f, 0.f, 0.f);
    angularAcceleration = glm::vec3(0.f, 0.f, 0.f);
    velocityDragFactor = 0.0f;
    angularDragFactor = 1.f;
    gravityFactor = 0.f;
    momentOfInertia = glm::vec3(1.f);
}

Physics::~Physics() {
}
