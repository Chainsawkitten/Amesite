#include "Explode.hpp"

using namespace Component;

Explode::Explode(Entity* entity) : SuperComponent(entity) {
    lifeTime = 1.f;
    size = 1.f;
}

Explode::~Explode() {
}
