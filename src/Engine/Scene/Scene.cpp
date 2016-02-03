#include "Scene.hpp"

#include "../Entity/Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"
#include "../Component/Physics.hpp"

Scene::Scene() {

}

Scene::~Scene() {
    Clear();
}

Entity* Scene::CreateEntity() {
    Entity* entity = new Entity(this);
    mEntityVec.push_back(entity);
    return entity;
}

void Scene::Clear() {
    for (unsigned int i = 0; i < mEntityVec.size(); i++)
        delete mEntityVec.at(i);
    mEntityVec.clear();
    mEntityVec.shrink_to_fit();

    for (unsigned int i = 0; i < mLensComponentVec.size(); i++)
        delete mLensComponentVec.at(i);
    mLensComponentVec.clear();
    mLensComponentVec.shrink_to_fit();

    for (unsigned int i = 0; i < mTransformComponentVec.size(); i++)
        delete mTransformComponentVec.at(i);
    mTransformComponentVec.clear();
    mTransformComponentVec.shrink_to_fit();

    for (unsigned int i = 0; i < mMeshComponentVec.size(); i++)
        delete mMeshComponentVec.at(i);
    mMeshComponentVec.clear();
    mMeshComponentVec.shrink_to_fit();

    for (unsigned int i = 0; i < mPhysicsComponentVec.size(); i++)
        delete mPhysicsComponentVec.at(i);
    mPhysicsComponentVec.clear();
    mPhysicsComponentVec.shrink_to_fit();
}

void Scene::UpdateModelMatrices() {
    for (unsigned int i = 0; i < mTransformComponentVec.size(); i++)
        mTransformComponentVec.at(i)->UpdateModelMatrix();
}