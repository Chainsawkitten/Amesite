#include "GridCollide.hpp"

using namespace Component;

GridCollide::GridCollide(Entity* entity) : SuperComponent(entity) {
    removeOnImpact = true;
    hasCollided = false;
}

GridCollide::~GridCollide() {
}
