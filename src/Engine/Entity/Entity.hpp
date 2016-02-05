#pragma once

#include <unordered_map>
#include <typeinfo>
#include "../Component/RelativeTransform.hpp"
#include "../Scene/Scene.hpp"

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

        /// Remove entity from scene.
        void Clear();

        /// Unordered map containing components.
        /**
         * Maps component type to component.
         */
        std::unordered_map<const std::type_info*, Component::SuperComponent*> components; //TODO MAP

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
        void AddComponent(Component::SuperComponent* component, const std::type_info* componentType);
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
    T* component = new T(this);
    const std::type_info* componentType = &typeid(component);
    AddComponent(component, componentType); // map
    mScene->AddComponentToList(component, componentType); // add to vector
    return component;
}

template <typename T> void Entity::RemoveComponent() {
    const std::type_info* componentType = &typeid(T*);

    if (components.find(componentType) != components.end()) {
        delete components[componentType]; // remove elm
        components.erase(componentType); // remove key
        mScene->RemoveComponentFromList(component, componentType);
    }
}
