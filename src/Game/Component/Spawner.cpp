#include "Spawner.hpp"

using namespace Component;

Spawner::Spawner(Entity* entity) : SuperComponent(entity) {
    timeSinceSpawn = 0.f;
    delay = 0.f;
    faction = 0;
    type = BULLET;
    enemyType = PYLON;
}

Spawner::~Spawner() {
    
}
