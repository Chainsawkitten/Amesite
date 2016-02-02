#include "Collider2DRectangle.hpp"
#include "Collider2DCircle.hpp"
#include "../Entity/Entity.hpp"

using namespace Component;

Collider2DRectangle::Collider2DRectangle(Entity* entity) : SuperComponent(entity) {
    width = height = 0.f;

}


Collider2DRectangle::~Collider2DRectangle() {

}