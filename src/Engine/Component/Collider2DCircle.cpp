#include "Collider2DCircle.hpp"

using namespace Component;

Collider2DCircle::Collider2DCircle(Entity* entity) : SuperComponent(entity) {
    radius = 1.f;
}

Collider2DCircle::~Collider2DCircle() {

}
