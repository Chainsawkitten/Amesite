#include "Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"

Entity::Entity() {
    mLens = nullptr;
    mTransform = nullptr;
}

void Entity::CreateLens() {
    mLens = new Component::Lens();
}

void Entity::CreateTransform() {
    mTransform = new Component::Transform();
}


Entity::~Entity() {
    if (mLens != nullptr)
        delete mLens;

    if (mTransform != nullptr)
        delete mTransform;
}