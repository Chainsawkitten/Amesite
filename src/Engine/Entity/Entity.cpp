#include "Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"

Entity::Entity() {
    mLens = nullptr;
    mTransform = nullptr;
}

Entity::~Entity() {
    if (mLens != nullptr)
        delete mLens;

    if (mTransform != nullptr)
        delete mTransform;
}

Component::Lens* Entity::CreateLens() {
    if (mLens == nullptr)
        return mLens = new Component::Lens();
    return nullptr;
}

Component::Transform* Entity::CreateTransform() {
    if (mTransform == nullptr)
        return mTransform = new Component::Transform();
    return nullptr;
}
