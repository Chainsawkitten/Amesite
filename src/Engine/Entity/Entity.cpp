#include "Entity.hpp"

#include "../Scene/Scene.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/RelativeTransform.hpp"
#include "../Component/Physics.hpp"
#include "../Component/Collider2DCircle.hpp"
#include "../Component/Collider2DRectangle.hpp"
#include "../Component/SuperComponent.hpp"

using namespace Component;

Entity::Entity(Scene* scene) {
    mScene = scene;
}

Entity::~Entity() {
}

void Entity::AddComponent(Component::SuperComponent* component, const type_info* componentType) {
	this->components[componentType] = component;
}