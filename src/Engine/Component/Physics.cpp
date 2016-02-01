#include "Physics.hpp"

using namespace Component;

Physics::Physics(Entity* entity) : SuperComponent(entity) {
    velocity = glm::vec3(0.f, 0.f, 0.f);
    maxVelocity = 20.f;
    angularVelocity = glm::vec3(0.f, 0.f, 0.f);
    maxAngularVelocity = 2.f;
    acceleration = glm::vec3(0.f, 0.f, 0.f);
    angularaAcceleration = glm::vec3(0.f, 0.f, 0.f);
    mass = 1.f;
    frictionCoefficient = 0.5f;
    angularDragFactor = 0.5f;
    gravityCoefficient = 1.f;
    momentOfInertia = glm::vec3(1.f);
}

Physics::~Physics() {
}
