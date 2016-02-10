#pragma once

class Entity;
class Scene;

#include <vector>
#include <map>
#include <string>

namespace GameObject {
    /// Game object containing a vector of entites which all game objects generalize from.
    class SuperGameObject {
        public:
            /// Create %SuperGameObject
            /**
             * @param scene Pointer to which Scene %SuperGameObject Enities' are contained.
             */
            SuperGameObject(Scene* scene);

            /// Destructor.
            virtual ~SuperGameObject();

            /// Remove %SuperGameObject from scene.
            void Clear();

            /// Get Entity from map.
            /**
             * @param key Key to Entity.
             * @return Return Enitity or nullptr.
             */
            Entity* GetEntity(const std::string key);

        protected:
            // Pointer to which Scene %Entity is contained.
            /**
             * Default: Must point to a Scene
             */
            Scene* mScene;

            // List of all entities in this %SuperGameObject.
            std::vector<Entity*> mEntityVector;

            // Map of entities in this %SuperGameObject.
            std::map<std::string, Entity*> mEntityMap;
    };
}
