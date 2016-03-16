#include "Scene.hpp"

#include "../Entity/Entity.hpp"
#include "../GameObject/SuperGameObject.hpp"
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
    std::vector<Component::Transform*> transforms = GetAll<Component::Transform>();
    for (Component::Transform* transform : transforms)
        transform->UpdateModelMatrix();
    
    std::vector<Component::Animation*> animations = GetAll<Component::Animation>();
    for (auto animationComponent : animations) {
        Component::RelativeTransform* relativeTranform = animationComponent->entity->GetComponent<Component::RelativeTransform>();
        if(relativeTranform != nullptr){
            Component::Animation* parentAnimation = relativeTranform->parentEntity->GetComponent<Component::Animation>();
            if (parentAnimation != nullptr) {
                Component::Transform* transformComponent = animationComponent->entity->GetComponent<Component::Transform>();
                transformComponent->worldOrientationMatrix = animationComponent->orientationMatrix * parentAnimation->orientationMatrix * transformComponent->worldOrientationMatrix;
                transformComponent->modelMatrix = animationComponent->animationMatrix * parentAnimation->animationMatrix * transformComponent->modelMatrix;
                animationComponent->orientationMatrix = parentAnimation->orientationMatrix;
                animationComponent->animationMatrix = parentAnimation->animationMatrix;
            } else {
                if (animationComponent->GetActiveAnimationClip() != nullptr) {
                    Component::Transform* transformComponent = animationComponent->entity->GetComponent<Component::Transform>();
                    transformComponent->worldOrientationMatrix = animationComponent->orientationMatrix * transformComponent->worldOrientationMatrix;
                    transformComponent->modelMatrix = animationComponent->animationMatrix * transformComponent->modelMatrix;
                }
            }
        }
    }
}

void Scene::ClearKilled() {
    // Clear killed components.
    std::size_t i;
    for (auto& componentIt : mComponents) {
        i = 0;
        while (i < componentIt.second.size()) {
            if (componentIt.second[i]->IsKilled()) {
                delete componentIt.second[i];
                componentIt.second[i] = componentIt.second[componentIt.second.size() - 1];
                componentIt.second.pop_back();
            } else {
                ++i;
            }
        }
    }
    
    // Clear killed entities.
    i = 0;
    while (i < mEntities.size()) {
        if (mEntities[i]->IsKilled()) {
            delete mEntities[i];
            mEntities[i] = mEntities[mEntities.size() - 1];
            mEntities.pop_back();
        } else {
            ++i;
        }
    }
    
    // Clear killed game objects.
    i = 0;
    while (i < mGameObjects.size()) {
        if (mGameObjects[i]->IsKilled()) {
            delete mGameObjects[i];
            mGameObjects[i] = mGameObjects[mGameObjects.size() - 1];
            mGameObjects.pop_back();
        } else {
            ++i;
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
