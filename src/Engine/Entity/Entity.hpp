#pragma once

namespace Component {
    class Transform;
    class Lens;
}

class Entity {
    public:
        /// Create a new Entity.
        Entity();

        /// Destructor.
        ~Entity();

        /// Add Component to Entity
        /**
        * @return Return new Component or nullptr if Entity allready contains the Component.
        */
        template<typename T> T* AddComponent() { return nullptr; }
        template<> Component::Lens* AddComponent<Component::Lens>() { return CreateLens(); }
        template<> Component::Transform* AddComponent<Component::Transform>() { return CreateTransform(); }

        /// Get Component of Entity
        /**
        * @return Return Component or nullptr if Entity doesn't contains the Component.
        */
        template<typename T> T* GetComponent() { return nullptr; }
        template<> Component::Lens* GetComponent<Component::Lens>() { return mLens; }
        template<> Component::Transform* GetComponent<Component::Transform>() { return mTransform; }

    private:
        Component::Lens* CreateLens();
        Component::Transform* CreateTransform();

        Component::Lens* mLens;
        Component::Transform* mTransform;
};