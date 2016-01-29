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
        mLens = new Component::Lens();
        mScene->mLensComponentVec.push_back(mLens);
        return mLens;
    }
    return nullptr;
}

Component::Transform* Entity::CreateTransform() {
    if (mTransform == nullptr) {
        mTransform = new Component::Transform();
        mScene->mTransformComponentVec.push_back(mTransform);
        return mTransform;
    }
    return nullptr;
}

Component::Mesh* Entity::CreateMesh() {
    if (mMesh == nullptr) {
        mMesh = new Component::Mesh();
        mScene->mMeshComponentVec.push_back(mMesh);
        return mMesh;
    }    
    return nullptr;
}

Component::RelativeTransform* Entity::CreateRelativeTransform() {
    if (mTransform == nullptr) {
        mRelativeTransform = new Component::RelativeTransform();
        mTransform = mRelativeTransform;
        //mScene->mTransformComponentVec.push_back(mTransform); || mScene->mTransformComponentVec.push_back(mRelativeTransform); ??
        mScene->mTransformComponentVec.push_back(mRelativeTransform);
        return mRelativeTransform;
    }
    return nullptr;
}
