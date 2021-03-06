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
        virtual ~Scene();
        
        /// Create new Entity in the scene.
        Entity* CreateEntity();
        
        /// Clear the scene of everything.
        void ClearAll();
        
        /// Updates all model matrices in %Scene.
        void UpdateModelMatrices();
        
        /// Removes all killed game objects, entities and components in the scene.
        void ClearKilled();
        
        /// Gets all components of a specific type.
        /**
         * @return A list of pointers to all components of the specified scene.
         */
        template <typename T> std::vector<T*>& GetAll();
        
        /// Gets all item of a specific type.
        /**
         * @return A pointer to a list of pointers to all items of the specified scene.
         */
        template <typename T> std::vector<T*>* GetVector() { return nullptr; }
        
        /// Updates %Scene by calling systems.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        virtual void Update(float deltaTime) = 0;
        
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
            std::vector<Entity*> intersect;
        };
        
    private:
        // Adds component to list internally.
        void AddComponentToList(Component::SuperComponent* component, const std::type_info* componentType);
        
        // Removes component from list internally.
        void RemoveComponentFromList(Component::SuperComponent* component, const std::type_info* componentType);
        
        // List of all entities created in this scene.
        std::vector<Entity*> mEntities;
        
        // All particles in the scene.
        System::ParticleSystem::Particle* mParticles;
        unsigned int mParticleCount;
        
        // Map containing list of components.
        std::map<const std::type_info*, std::vector<Component::SuperComponent*>> mComponents;
        
        // List of all collisons in this scene.
        std::vector<Collision*> mCollisions;
        
        // List of all game objects in this scene.
        std::vector<GameObject::SuperGameObject*> mGameObjects;
};

// GetAll<T>
template <typename T> inline std::vector<T*>& Scene::GetAll() {
    return reinterpret_cast<std::vector<T*>&>(mComponents[&typeid(T*)]);
}

// GetVector<T>
template<> inline std::vector<Entity*>* Scene::GetVector() {
    return &mEntities;
}

template<> inline std::vector<GameObject::SuperGameObject*>* Scene::GetVector() {
    return &mGameObjects;
}

template<> inline std::vector<Scene::Collision*>* Scene::GetVector() {
    return &mCollisions;
}
