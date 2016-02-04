#include "Damage.hpp"

using namespace Component;

Damage::Damage(Entity* entity) : SuperComponent(entity) {
    damageAmount = 10.f;
}

Damage::~Damage() {
}
