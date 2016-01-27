#include "Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"

Entity::Entity() {
    mLens = nullptr;
    mTransform = nullptr;
    mMesh = nullptr;
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

Component::Mesh* Entity::CreateMesh() {
    if (mMesh == nullptr)
        return mMesh = new Component::Mesh();
    return nullptr;
}



Entity::~Entity() {
    if (mLens != nullptr)
        delete mLens;

    if (mTransform != nullptr)
        delete mTransform;

    if (mMesh != nullptr)
        delete mMesh;
}