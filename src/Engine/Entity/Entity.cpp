#include "Entity.hpp"

#include "../Scene/Scene.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"

Entity::Entity(Scene* scene) {
    mScene = scene;

    mLens = nullptr;
    mTransform = nullptr;
    mMesh = nullptr;
    mRelativeTransform = nullptr;
}

Entity::~Entity() {
}

Component::Lens* Entity::CreateLens() {
    if (mLens == nullptr) {
        mLens = new Component::Lens(this);
        mScene->mLensComponentVec.push_back(mLens);
        return mLens;
    }
    return nullptr;
}

Component::Transform* Entity::CreateTransform() {
    if (mTransform == nullptr) {
        mTransform = new Component::Transform(this);
        mScene->mTransformComponentVec.push_back(mTransform);
        return mTransform;
    }
    return nullptr;
}

Component::Mesh* Entity::CreateMesh() {
    if (mMesh == nullptr) {
        mMesh = new Component::Mesh(this);
        mScene->mMeshComponentVec.push_back(mMesh);
        return mMesh;
    }    
    return nullptr;
}

Component::RelativeTransform* Entity::CreateRelativeTransform() {
    if (mTransform == nullptr) {
        mRelativeTransform = new Component::RelativeTransform(this);
        mTransform = mRelativeTransform;
        mScene->mTransformComponentVec.push_back(mRelativeTransform);
        return mRelativeTransform;
    }
    return nullptr;
}
