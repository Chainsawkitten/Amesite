#pragma once

#include <unordered_map>
#include <typeinfo>
#include "../Util/log.hpp"
#include "../Component/RelativeTransform.hpp"
#include <Engine/Scene/Scene.hpp>

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
    class RelativeTransform;
    class Physics;
    class Collider2DRectangle;
    class Collider2DCircle;
    class SuperComponent;
}

class Scene;

class Entity {
    public:
        /// Create new Entity.
        /**
         * @param scene Pointer to which Scene %Entity is contained.
         */
        Entity(Scene* scene);

        /// Destructor.
        ~Entity();

		///Adds component with type T
        template <typename T> T* AddComponent();

		///Gets component with type T
        template <typename T> T* GetComponent();

		///Unordered map containing components.
		/**
		 * Maps component type to component.
		 */
        std::unordered_map<const std::type_info*, Component::SuperComponent*> components;

    private:
        /// Pointer to which Scene %Entity is contained.
        /**
         * Default: Must point to a Scene
         */
        Scene* mScene;

		///Adds a component to this entity and to the scene.
		/**
		 * @param component The component that will be added.
		 * @param componentType The type of the component.
		 */
		void AddComponent(Component::SuperComponent* component, const type_info* componentType);
};


template <typename T> T* Entity::GetComponent() {
	if (this->components.count(&typeid(T*)) != 0) {
		return static_cast<T*>(this->components[&typeid(T*)]);
	} else {
		return nullptr;
	}
}

template <> inline Component::RelativeTransform* Entity::AddComponent<Component::RelativeTransform>() {
        if(this->GetComponent<Component::Transform>() == nullptr && this->GetComponent<Component::RelativeTransform>() == nullptr) {
            Component::RelativeTransform* mRelativeTransform = new Component::RelativeTransform(this);
            
            AddComponent(mRelativeTransform, &typeid(Component::Transform*));
            AddComponent(mRelativeTransform, &typeid(mRelativeTransform));

            mScene->AddComponentToList(mRelativeTransform, &typeid(Component::Transform*));
            mScene->AddComponentToList(mRelativeTransform, &typeid(mRelativeTransform));
            return mRelativeTransform;
        }
    return nullptr;
}

template <typename T> T* Entity::AddComponent() {
	T* component = new T(this);
	const type_info* componentType = &typeid(component);
	AddComponent(component, componentType);
	mScene->AddComponentToList(component, componentType);
	return component;
}