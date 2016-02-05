#include "Scene.hpp"

#include "../Entity/Entity.hpp"
#include <algorithm>

Scene::Scene() {
    mParticlesVector = new std::vector<System::ParticleSystem::Particle>;
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

    for (auto& it : mComponents) {
        for (Component::SuperComponent* component : it.second)
            delete component;
    }
    mComponents.clear();
    delete mParticlesVector;
}

void Scene::UpdateModelMatrices() {
    std::vector<Component::Transform*> transforms = GetAll<Component::Transform>();
    for (unsigned int i = 0; i < transforms.size(); i++)
        transforms[i]->UpdateModelMatrix();
}

void Scene::RemoveEntity(Entity* entity) {
    // Remove entity's components
    for (auto& it : mComponents) {
        if (entity->components[it.first] != nullptr) {
            it.second.erase(std::remove(it.second.begin(), it.second.end(), entity->components[it.first]), it.second.end());
            delete entity->components[it.first];
        }
    }

    //Remove Entity
    mEntityVector.erase(std::remove(mEntityVector.begin(), mEntityVector.end(), entity), mEntityVector.end());
    delete entity;
}
