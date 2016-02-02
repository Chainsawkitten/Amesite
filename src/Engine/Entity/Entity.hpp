#pragma once

#include <unordered_map>
#include <typeinfo>
#include "../Util/log.hpp"

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
    class RelativeTransform;
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

        template <typename T>
        T* AddComponent() {
            T* component = new T(this);
            const type_info* componentType = &typeid(component);
            AddComponent(component, componentType);
            return component;
        }

        void AddComponent(Component::SuperComponent* component, const type_info* componentType) {
            Log() << componentType->name();
            this->components[componentType] = component;
        }

        template <typename T>
        T* GetComponent() {
            if (this->components.count(&typeid(T*)) != 0) {
                return static_cast<T*>(this->components[&typeid(T*)]);
            } else {
                return nullptr;
            }
        }

        std::unordered_map<const std::type_info*, Component::SuperComponent*> components;
    private:
        /// Pointer to which Scene %Entity is contained.
        /**
         * Default: Must point to a Scene
         */
        Scene* mScene;
};