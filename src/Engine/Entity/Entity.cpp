#include "Entity.hpp"

#include "../Scene/Scene.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"
#include "../Component/Physics.hpp"
#include "../Component/Collider2DCircle.hpp"
#include "../Component/Collider2DRectangle.hpp"
#include "../Component/ParticleEmitter.hpp"

using namespace Component;

Entity::Entity(Scene* scene) {
    mScene = scene;

    mLens = nullptr;
    mTransform = nullptr;
    mMesh = nullptr;
    mRelativeTransform = nullptr;
    mPhysics = nullptr;
    mCircle = nullptr;
    mRectangle = nullptr;
    mParticle = nullptr;
}

Entity::~Entity() {
}

Lens* Entity::CreateLens() {
    if (mLens == nullptr) {
        mLens = new Component::Lens(this);
        mScene->mLensComponentVec.push_back(mLens);
        return mLens;
    }
    return nullptr;
}

Transform* Entity::CreateTransform() {
    if (mTransform == nullptr) {
        mTransform = new Component::Transform(this);
        mScene->mTransformComponentVec.push_back(mTransform);
        return mTransform;
    }
    return nullptr;
}

Mesh* Entity::CreateMesh() {
    if (mMesh == nullptr) {
        mMesh = new Component::Mesh(this);
        mScene->mMeshComponentVec.push_back(mMesh);
        return mMesh;
    }    
    return nullptr;
}

RelativeTransform* Entity::CreateRelativeTransform() {
    if (mTransform == nullptr) {
        mRelativeTransform = new Component::RelativeTransform(this);
        mTransform = mRelativeTransform;
        mScene->mTransformComponentVec.push_back(mRelativeTransform);
        return mRelativeTransform;
    }
    return nullptr;
}

Component::Physics* Entity::CreatePhysics() {
    if (mPhysics == nullptr) {
        mPhysics = new Component::Physics(this);
        mScene->mPhysicsComponentVec.push_back(mPhysics);
        return mPhysics;
    }
    return nullptr;
}

Collider2DCircle* Entity::CreateCollider2DCircle() {
    if (mCircle == nullptr) {
        mCircle = new Component::Collider2DCircle(this);
        return mCircle;
    }
    return nullptr;
}

Collider2DRectangle* Entity::CreateCollider2DRectangle() {
    if (mRectangle == nullptr) {
        mRectangle = new Component::Collider2DRectangle(this);
        return mRectangle;
    }
    return nullptr;
}

ParticleEmitter* Entity::CreateParticleEmitter()
{
    if (mParticle == nullptr) {
        mParticle = new Component::ParticleEmitter(this);
        return mParticle;
    }
    return nullptr;
}
