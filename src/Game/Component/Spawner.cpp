#include "Spawner.hpp"

using namespace Component;

Spawner::Spawner(Entity* entity) : SuperComponent(entity) {
    timeSinceSpawn = 0.0f;
    delay = 0.0f;
}

Spawner::~Spawner() {
    
}
