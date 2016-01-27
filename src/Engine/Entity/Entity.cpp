#include "Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"

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

void Entity::CreateMesh() {
    mMesh = new Component::Mesh();
}


Entity::~Entity() {
    if (mLens != nullptr)
        delete mLens;

    if (mTransform != nullptr)
        delete mTransform;
}