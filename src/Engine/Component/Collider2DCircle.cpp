#include "Collider2DCircle.hpp"

using namespace Component;

Collider2DCircle::Collider2DCircle(Entity* entity) : SuperComponent(entity) {
    radius = 0.f;
}

Collider2DCircle::~Collider2DCircle() {

}
