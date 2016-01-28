#include "Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"

Entity::Entity() {
    mLens = nullptr;
    mTransform = nullptr;
    mMesh = nullptr;
}

Entity::~Entity() {
    if (mLens != nullptr)
        delete mLens;

    if (mTransform != nullptr)
        delete mTransform;
    
    if (mMesh != nullptr)
        delete mMesh;
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

Component::RelativeTransform* Entity::CreateRelativeTransform() {
    //if (mTransofmr == nullptr) {
    //    mRelativeTransform = new RelativeTransform();
    //    return mTransform = mRelativeTransform;
    //}
    return nullptr;
}
