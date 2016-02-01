#pragma once

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
    class RelativeTransform;
    class Collider2DRectangle;
    class Collider2DCircle;
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
        Component::Collider2DCircle* CreateCollider2DCircle();
        Component::Collider2DRectangle* CreateCollider2DRectangle();

        Component::Lens* mLens;
        Component::Transform* mTransform;
        Component::Mesh* mMesh;
        Component::RelativeTransform* mRelativeTransform;
        Component::Collider2DCircle* mCircle;
        Component::Collider2DRectangle* mRectangle;

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

template<> inline Component::Collider2DCircle* Entity::AddComponent<Component::Collider2DCircle>() {
    return CreateCollider2DCircle();
}

template<> inline Component::Collider2DRectangle* Entity::AddComponent<Component::Collider2DRectangle>() {
    return CreateCollider2DRectangle();
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


template<> inline Component::Collider2DCircle* Entity::GetComponent<Component::Collider2DCircle>() {
    return mCircle;
}

template<> inline Component::Collider2DRectangle* Entity::GetComponent<Component::Collider2DRectangle>() {
    return mRectangle;
}