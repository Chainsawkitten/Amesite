#include "GroundCollide.hpp"

using namespace Component;

GroundCollide::GroundCollide(Entity* entity) : SuperComponent(entity) {
    removeOnImpact = true;
    hasCollided = false;
}

GroundCollide::~GroundCollide() {
}
