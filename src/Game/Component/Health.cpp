#include "Health.hpp"

using namespace Component;

Health::Health(Entity* entity) : SuperComponent(entity) {
    health = 100.f;
    maxHealth = 100.f;
    toughness = 1.f;
    maxCooldown = 1.f;
    regenAmount = 1.f;
    cooldown = 0.f;
    faction = 0;
    removeOnLowHealth = true;
    damaged = -1;
}

Health::~Health() {
}
