#include "Reflect.hpp"

using namespace Component;

Reflect::Reflect(Entity* entity) : SuperComponent(entity) {
    faction = 1;
}

Reflect::~Reflect() {
}
