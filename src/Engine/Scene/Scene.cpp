#include "Scene.hpp"

#include "../Entity/Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"
#include "../Component/SuperComponent.hpp"
#include "../Component/Physics.hpp"
#include "../Component/ParticleEmitter.hpp"

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

void Scene::ClearAll() {
    for (Entity* entity : mEntityVector) {
        delete entity;
    }
    mEntityVector.clear();
    mEntityVector.shrink_to_fit();

    for (Collision* collision : mCollisionVector) {
        delete collision;
    }
    mCollisionVector.clear();
    mCollisionVector.shrink_to_fit();

    for (auto it : mComponents) {
        for (Component::SuperComponent* component : it.second) {
            delete component;
        }
        it.second.clear();
        it.second.shrink_to_fit();
    }
    mComponents.clear();
}

void Scene::UpdateModelMatrices() {
    std::vector<Component::Transform*> transforms = GetAll<Component::Transform>();
    for (unsigned int i = 0; i < transforms.size(); i++)
        transforms[i]->UpdateModelMatrix();
}
