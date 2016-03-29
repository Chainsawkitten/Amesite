#include "LifeTime.hpp"

using namespace Component;

LifeTime::LifeTime(Entity* entity) : SuperComponent(entity) {
    initialLifeTime = lifeTime = 10.f;
}

LifeTime::~LifeTime() {
}
