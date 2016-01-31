#pragma once

class Entity;

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
    class RelativeTransform;
}

#include <vector>

/// Contains a bunch of entities.
class Scene {

    /// %Scene is friend with Enity to gain access to components
    friend class Entity;

    public:
        /// Create new Scene.
        Scene();

        /// Destructor.
        ~Scene();

        /// Create new Entity in Scene.
        Entity* CreateEntity();

        /// Get number of items in %Scene.
        /**
        * @return Number of items in %Scene, returns -1 if no such item can be found.
        */
        template<typename T> unsigned int Size() const { return 0; }

        /// Clear Scene of all items.
        void Clear();

        /// Get item on index, else nullptr.
        /**
        * @param index The index of the item.
        * @return Pointer to item on index, else nullptr.
        */
        template<typename T> T* Get(unsigned int index) const { return nullptr; }

    private:
        std::vector<Entity*> mEntityVec;

        std::vector<Component::Lens*> mLensComponentVec;
        std::vector<Component::Transform*> mTransformComponentVec;
        std::vector<Component::Mesh*> mMeshComponentVec;
};

// Size<T>()
template<> inline unsigned int Scene::Size<Entity>() const {
    return mEntityVec.size();
}

template<> inline unsigned int Scene::Size<Component::Lens>() const {
    return mLensComponentVec.size();
}

template<> inline unsigned int Scene::Size<Component::Transform>() const {
    return mTransformComponentVec.size();
}

template<> inline unsigned int Scene::Size<Component::Mesh>() const {
    return mMeshComponentVec.size();
}

// Get<T>()
template<> inline Entity* Scene::Get(unsigned int index) const {
    if (index < mEntityVec.size())
        return mEntityVec.at(index);
    return nullptr;
}

template<> inline Component::Lens* Scene::Get(unsigned int index) const {
    if (index < mLensComponentVec.size())
        return mLensComponentVec.at(index);
    return nullptr;
}

template<> inline Component::Transform* Scene::Get(unsigned int index) const {
    if (index < mTransformComponentVec.size())
        return mTransformComponentVec.at(index);
    return nullptr;
}

template<> inline Component::Mesh* Scene::Get(unsigned int index) const {
    if (index < mMeshComponentVec.size())
        return mMeshComponentVec.at(index);
    return nullptr;
}
