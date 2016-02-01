#pragma once

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
    class RelativeTransform;
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

        /// Add Component to Entity.
        /**
         * @return New Component or nullptr if Entity already contains the Component.
         */
        template<typename T> T* AddComponent() { return nullptr; }

        /// Get Component of Entity.
        /**
         * @return Component or nullptr if Entity doesn't contains the Component.
         */
        template<typename T> T* GetComponent() { return nullptr; }

    private:
        Component::Lens* CreateLens();
        Component::Transform* CreateTransform();
        Component::Mesh* CreateMesh();
        Component::RelativeTransform* CreateRelativeTransform();

        Component::Lens* mLens;
        Component::Transform* mTransform;
        Component::Mesh* mMesh;
        Component::RelativeTransform* mRelativeTransform;

        /// Pointer to which Scene %Entity is contained.
        /**
         * Default: Must point to a Scene
         */
        Scene* mScene;
};

// AddComponent<T>()
template<> inline Component::Lens* Entity::AddComponent<Component::Lens>() {
    return CreateLens();
}

template<> inline Component::Transform* Entity::AddComponent<Component::Transform>() {
    return CreateTransform();
}

template<> inline Component::Mesh* Entity::AddComponent<Component::Mesh>() {
    return CreateMesh();
}

template<> inline Component::RelativeTransform* Entity::AddComponent<Component::RelativeTransform>() {
    return CreateRelativeTransform();
}

// GetComponent<T>()
template<> inline Component::Lens* Entity::GetComponent<Component::Lens>() {
    return mLens;
}

template<> inline Component::Transform* Entity::GetComponent<Component::Transform>() {
    return mTransform;
}

template<> inline Component::Mesh* Entity::GetComponent<Component::Mesh>() {
    return mMesh;
}

template<> inline Component::RelativeTransform* Entity::GetComponent<Component::RelativeTransform>() {
    return mRelativeTransform;
}
