#include "Scene.hpp"

#include "../Entity/Entity.hpp"
#include <Engine/GameObject/SuperGameObject.hpp>
#include "../Component/Animation.hpp"

#include <algorithm>

Scene::Scene() {
    mParticles = new System::ParticleSystem::Particle[System::ParticleSystem::GetActiveInstance().MaxParticleCount()];
    mParticleCount = 0;
}

Scene::~Scene() {
    ClearAll();
    
    delete[] mParticles;
}

Entity* Scene::CreateEntity() {
    Entity* entity = new Entity(this);
    mEntities.push_back(entity);
    return entity;
}

void Scene::AddComponentToList(Component::SuperComponent* component, const std::type_info* componentType) {
    mComponents[componentType].push_back(component);
}

void Scene::RemoveComponentFromList(Component::SuperComponent* component, const std::type_info* componentType) {
    mComponents[componentType].erase(std::remove(mComponents[componentType].begin(), mComponents[componentType].end(), component), mComponents[componentType].end());
}

void Scene::ClearAll() {
    for (Entity* entity : mEntities)
        delete entity;
    mEntities.clear();
    
    for (Collision* collision : mCollisions)
        delete collision;
    mCollisions.clear();
    
    for (GameObject::SuperGameObject* gameObject : mGameObjects)
        delete gameObject;
    mGameObjects.clear();
    
    for (auto& it : mComponents) {
        for (Component::SuperComponent* component : it.second)
            delete component;
    }
    mComponents.clear();
    
    mParticleCount = 0;
}

void Scene::UpdateModelMatrices() {
    std::list<Component::Transform*> transforms = GetAll<Component::Transform>();
    for (Component::Transform* transform : transforms)
        transform->UpdateModelMatrix();
    
    std::list<Component::Animation*> animations = GetAll<Component::Animation>();
    for (auto animationComponent : animations) {
        Component::RelativeTransform* relativeTranform = animationComponent->entity->GetComponent<Component::RelativeTransform>();
        if (relativeTranform != nullptr && relativeTranform->parentEntity->GetComponent<Component::Animation>() != nullptr) {
            Component::Animation* relativeAnimation = relativeTranform->parentEntity->GetComponent<Component::Animation>();
            animationComponent->entity->GetComponent<Component::Transform>()->worldOrientationMatrix = animationComponent->orientationMatrix * relativeAnimation->orientationMatrix * animationComponent->entity->GetComponent<Component::Transform>()->worldOrientationMatrix;
            animationComponent->entity->GetComponent<Component::Transform>()->modelMatrix = animationComponent->animationMatrix * relativeAnimation->animationMatrix * animationComponent->entity->GetComponent<Component::Transform>()->modelMatrix;
            animationComponent->orientationMatrix = relativeTranform->parentEntity->GetComponent<Component::Animation>()->orientationMatrix;
            animationComponent->animationMatrix = relativeTranform->parentEntity->GetComponent<Component::Animation>()->animationMatrix;
        } else {
            if (animationComponent->GetActiveAnimationClip() != nullptr) {
                animationComponent->entity->GetComponent<Component::Transform>()->worldOrientationMatrix = animationComponent->orientationMatrix * animationComponent->entity->GetComponent<Component::Transform>()->worldOrientationMatrix;
                animationComponent->entity->GetComponent<Component::Transform>()->modelMatrix = animationComponent->animationMatrix * animationComponent->entity->GetComponent<Component::Transform>()->modelMatrix;
            }
        }
    }
}

void Scene::ClearKilled() {
    // Clear killed components.
    for (auto& componentIt : mComponents) {
        auto it = componentIt.second.begin();
        while (it != componentIt.second.end()) {
            if ((*it)->IsKilled()) {
                delete *it;
                it = componentIt.second.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    // Clear killed entities.
    auto it = mEntities.begin();
    while (it != mEntities.end()) {
        if ((*it)->IsKilled()) {
            delete *it;
            it = mEntities.erase(it);
        } else {
            ++it;
        }
    }
}

System::ParticleSystem::Particle* Scene::GetParticles() const {
    return mParticles;
}

unsigned int Scene::GetParticleCount() const {
    return mParticleCount;
}

void Scene::SetParticleCount(unsigned int particleCount) {
    mParticleCount = particleCount;
}
