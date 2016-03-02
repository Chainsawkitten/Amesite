#pragma once

#include <map>
#include <typeinfo>
#include <vector>
#include <list>
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
        virtual ~Scene();
        
        /// Create new Entity in the scene.
        Entity* CreateEntity();
        
        /// Clear the scene of everything.
        void ClearAll();
        
        /// Updates all model matrices in %Scene.
        void UpdateModelMatrices();
        
        /// Removes all killed entites in %Scene.
        void ClearKilledEntities();
        
        /// Gets all components of a specific type.
        /**
         * @return A vector of pointers to all components of the specified scene.
         */
        template <typename T> std::vector<T*>& GetAll();
        
        /// Gets all item of a specific type.
        /**
         * @return A pointer to a list of pointers to all items of the specified scene.
         */
        template <typename T> std::list<T*>* GetList() { return nullptr; }
        
        /// Updates %Scene by calling systems.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        virtual void Update(float deltaTime) = 0;
        
        /// Gets a read-only vector of killed entites.
        /**
         * @return A pointer to a list of pointers to all killed entites.
         */
        const std::list<Entity*>& GetKilledEntitesVector() const;
        
        /// Get all the particles in the scene.
        /**
         * @return Array of all the particles in the scene.
         */
        System::ParticleSystem::Particle* GetParticles() const;
        
        /// Get the number of particles in the scene.
        /**
         * @return The number of particles in the scene.
         */
        unsigned int GetParticleCount() const;
        
        /// Set the number of particles in the scene.
        /**
         * @param particleCount The number of particles in the scene.
         */
        void SetParticleCount(unsigned int particleCount);
        
        /// Contains data about which entities in the scene this entity intersects with.
        struct Collision {
            /// The entity in question.
            Entity* entity = nullptr;

            /// Instersecting entities.
            std::list<Entity*> intersect;
        };
        
    private:
        // Adds component to list internally.
        void AddComponentToList(Component::SuperComponent* component, const std::type_info* componentType);
        
        // Removes component from list internally.
        void RemoveComponentFromList(Component::SuperComponent* component, const std::type_info* componentType);
        
        // List of all entities created in this scene.
        std::list<Entity*> mEntities;
        
        // All particles in the scene.
        System::ParticleSystem::Particle* mParticles;
        unsigned int mParticleCount;
        
        // Map containing vectors of components.
        std::map<const std::type_info*, std::vector<Component::SuperComponent*>> mComponents;
        
        // List of all collisons in this scene.
        std::list<Collision*> mCollisions;
        
        // List of all game objects in this scene.
        std::list<GameObject::SuperGameObject*> mGameObjects;
        
        // List of entites to be removed.
        std::list<Entity*> mKilledEntites;
};

// GetAll<T>
template <typename T> inline std::vector<T*>& Scene::GetAll() {
    return reinterpret_cast<std::vector<T*>&>(mComponents[&typeid(T*)]);
}

// GetList<T>
template<> inline std::list<Entity*>* Scene::GetList() {
    return &mEntities;
}

template<> inline std::list<GameObject::SuperGameObject*>* Scene::GetList() {
    return &mGameObjects;
}

template<> inline std::list<Scene::Collision*>* Scene::GetList() {
    return &mCollisions;
}
