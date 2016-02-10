#pragma once

#include <map>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include <iterator>
#include "../System/ParticleSystem.hpp"

class Entity;

namespace Component {
    class SuperComponent;
}

namespace GameObject {
    class SuperGameObject;
}

/// Contains a bunch of entities.
class Scene {

    /// %Scene is friend with Entity to gain access to components.
    friend class Entity;

    public:
        /// Create new Scene.
        Scene();

        /// Destructor.
        ~Scene();

        /// Create new Entity in the scene.
        Entity* CreateEntity();
        
        /// Clear the scene of everything.
        void ClearAll();
        
        /// Updates all model matrices in %Scene.
        void UpdateModelMatrices();

        /// Updates all animation matrices in %Scene
        void UpdateAnimationMatrices();

        /// Gets all components of a specific type.
        /**
         * @return A vector of pointers to all components of the specified scene.
         */
        template <typename T> std::vector<T*>& GetAll();

        /// Gets all item of a specific type.
        /**
         * @return A pointer to a vector of pointers to all items of the specified scene.
         */
        template <typename T> std::vector<T*>* GetVector() { return nullptr; }

        /// Gets all item of a specific type.
        /**
         * @return A pointer to a vector of pointers to all items of the specified scene.
         */
        template <typename T> std::vector<T>* GetVectorContents();

        /// Contains data about which entities in the scene this entity intersects with.
        struct Collision {
            Entity* entity = nullptr;
            std::vector<Entity*> intersect;
        };

        /// Removes Entity from %Scene
        /**
         *@param entity Entity to be removed.
         */
        void RemoveEntity(Entity* entity);

        virtual void Update(float deltaTime) = 0;

    private:
        // Adds component to list internally.
        void AddComponentToList(Component::SuperComponent* component, const std::type_info* componentType);

        // Removes component from list internally.
        void RemoveComponentFromList(Component::SuperComponent* component, const std::type_info* componentType);

        // List of all entities created in this scene.
        std::vector<Entity*> mEntityVector;
        
        // List of all particles in this scene.
        std::vector<System::ParticleSystem::Particle> mParticlesVector;

        // Map containing vectors of components.
        std::map<const std::type_info*, std::vector<Component::SuperComponent*>> mComponents;

        // List of all collisons in this scene.
        std::vector<Collision*> mCollisionVector;

        // List of all game objects in this scene.
        std::vector<GameObject::SuperGameObject*> mGameObjectVector;
};

// GetAll<T>
template <typename T> inline std::vector<T*>& Scene::GetAll() {
    return reinterpret_cast<std::vector<T*>&>(mComponents[&typeid(T*)]);
}

// GetVector<T>
template<> inline std::vector<Entity*>* Scene::GetVector() {
    return &mEntityVector;
}

template<> inline std::vector<GameObject::SuperGameObject*>* Scene::GetVector() {
    return &mGameObjectVector;
}

template<> inline std::vector<Scene::Collision*>* Scene::GetVector() {
    return &mCollisionVector;
}

// GetVectorContents<T>
template<> inline std::vector<System::ParticleSystem::Particle>* Scene::GetVectorContents() {
    return &mParticlesVector;
}
