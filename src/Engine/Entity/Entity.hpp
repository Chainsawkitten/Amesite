#pragma once

#include <map>
#include <typeinfo>
#include "../Component/RelativeTransform.hpp"
#include "../Scene/Scene.hpp"

namespace GameObject {
    class SuperGameObject;
}

/// %Entity containing various components.
class Entity {
    public:
        /// Create new Entity.
        /**
         * @param scene Pointer to which Scene %Entity is contained.
         */
        Entity(Scene* scene);

        /// Destructor.
        ~Entity();

        /// Adds component with type T.
        /**
         * @return The created component.
         */
        template <typename T> T* AddComponent();

        /// Gets component with type T.
        /**
         * @return The requested component (or nullptr).
         */
        template <typename T> T* GetComponent();

        /// Remove component of type T.
        template <typename T> void RemoveComponent();

        /// Remove %Entity from scene.
        void Clear();

        /// Kill entity, will be removed at end of frame by scene.
        void Kill();

        // Pointer to which GameObject %Entity is contained.
        /**
         * Default: nullptr
         */
        GameObject::SuperGameObject* gameObject;

    private:
        // Adds a component to this entity and to the scene.
        /**
         * @param component The component that will be added.
         * @param componentType The type of the component.
         */
        void AddComponent(Component::SuperComponent* component, const std::type_info* componentType);

        // Pointer to which Scene %Entity is contained.
        /**
         * Default: Must point to a Scene
         */
        Scene* mScene;

        // Unordered map containing components.
        /**
         * Maps component type to component.
         */
        std::map<const std::type_info*, Component::SuperComponent*> components;
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
            return mRelativeTransform;
        }
    return nullptr;
}

template <typename T> T* Entity::AddComponent() {
    const std::type_info* componentType = &typeid(T*);
    if (components.find(componentType) != components.end())
        return nullptr;
    T* component = new T(this);
    AddComponent(component, componentType);
    mScene->AddComponentToList(component, componentType);
    return component;
}

template <typename T> void Entity::RemoveComponent() {
    const std::type_info* componentType = &typeid(T*);
    if (components.find(componentType) != components.end()) {
        T* component = static_cast<T*>(components[componentType]);
        delete component;
        components.erase(componentType);
        mScene->RemoveComponentFromList(component, componentType);
    }
}
