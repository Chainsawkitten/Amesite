#include "Scene.hpp"

#include "../Entity/Entity.hpp"
#include <Engine/GameObject/SuperGameObject.hpp>
#include "../Component/Animation.hpp"

#include <algorithm>

Scene::Scene() {

}

Scene::~Scene() {
    ClearAll();
}

Entity* Scene::CreateEntity() {
    Entity* entity = new Entity(this);
    mEntityVector.push_back(entity);
    return entity;
}

void Scene::AddComponentToList(Component::SuperComponent* component, const std::type_info* componentType) {
    mComponents[componentType].push_back(component);
}

void Scene::RemoveComponentFromList(Component::SuperComponent* component, const std::type_info* componentType) {
    mComponents[componentType].erase(std::remove(mComponents[componentType].begin(), mComponents[componentType].end(), component), mComponents[componentType].end());
}

void Scene::ClearAll() {
    for (Entity* entity : mEntityVector)
        delete entity;
    mEntityVector.clear();
    mEntityVector.shrink_to_fit();

    for (Collision* collision : mCollisionVector)
        delete collision;
    mCollisionVector.clear();
    mCollisionVector.shrink_to_fit();

    for (GameObject::SuperGameObject* gameObject : mGameObjectVector)
        delete gameObject;
    mGameObjectVector.clear();
    mGameObjectVector.shrink_to_fit();

    for (auto& it : mComponents) {
        for (Component::SuperComponent* component : it.second)
            delete component;
    }
    mComponents.clear();

    mParticlesVector.clear();
    mParticlesVector.shrink_to_fit();
}

void Scene::UpdateModelMatrices() {
    std::vector<Component::Transform*> transforms = GetAll<Component::Transform>();
    for (unsigned int i = 0; i < transforms.size(); i++) {
        transforms[i]->UpdateModelMatrix();
    }
}

void Scene::UpdateAnimationMatrices() {
    std::vector<Component::Animation*> animationVector = GetAll<Component::Animation>();
    for (auto animationComponent : animationVector) {
        if (animationComponent->GetActiveAnimationClip() == nullptr) {
            animationComponent->orientationMatrix = animationComponent->entity->GetComponent<Component::Transform>()->orientationMatrix;
            animationComponent->animationMatrix = animationComponent->entity->GetComponent<Component::Transform>()->modelMatrix;
        } else {
            animationComponent->orientationMatrix = animationComponent->entity->GetComponent<Component::Transform>()->orientationMatrix * animationComponent->orientationMatrix;
            animationComponent->animationMatrix = animationComponent->entity->GetComponent<Component::Transform>()->modelMatrix * animationComponent->animationMatrix;
        }
    }
}

void Scene::RemoveEntity(Entity* entity) {
    entity->Clear();
    mEntityVector.erase(std::remove(mEntityVector.begin(), mEntityVector.end(), entity), mEntityVector.end());
    delete entity;
}
