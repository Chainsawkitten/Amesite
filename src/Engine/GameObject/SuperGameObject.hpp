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

            /// Kill game object, will be removed at end of frame by scene.
            void Kill();
            
            /// Get whether game object has been killed.
            /**
             * @return Whether the game object has been killed.
             */
            bool IsKilled() const;

        protected:
            /// Create entity belonging to the game object.
            /**
             * @return The created entity.
             */
            Entity* CreateEntity();

        private:
            // Pointer to which Scene %Entity is contained.
            /*
             * Default: Must point to a Scene
             */
            Scene* mScene;

            // List of all entities in this %SuperGameObject.
            std::vector<Entity*> mEntityVector;
            
            bool mKilled;
    };
}
