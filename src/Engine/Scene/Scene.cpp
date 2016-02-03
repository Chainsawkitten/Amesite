#include "Scene.hpp"

#include "../Entity/Entity.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"
#include "../Component/SuperComponent.hpp"
#include "../Component/Physics.hpp"

Scene::Scene() {

}

Scene::~Scene() {
    Clear();
}

Entity* Scene::CreateEntity() {
    Entity* entity = new Entity(this);
    mEntityVector.push_back(entity);
    return entity;
}

void Scene::AddComponentToList(Component::SuperComponent* component, const type_info* componentType) {
	std::pair<const std::type_info*, Component::SuperComponent*> pair(componentType, component);
	mComponents.insert(pair);
}

void Scene::Clear() {
	//TODO: CLEAR STUFF
}

void Scene::UpdateModelMatrices() {
	std::vector<Component::Transform*> transforms;
	GetAll<Component::Transform>(transforms);
	for (unsigned int i = 0; i < transforms.size(); i++)
		transforms[i]->UpdateModelMatrix();
        
}
