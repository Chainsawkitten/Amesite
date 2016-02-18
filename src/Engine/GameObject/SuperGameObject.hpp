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

        protected:
            // Create Entity to %GameObject.
            /**
             * @param scene The Scene this %GameObject will be contained.
             * @return Return Enitity.
             */
            Entity* CreateEntity(Scene* scene);

        private:
            // Pointer to which Scene %Entity is contained.
            /**
             * Default: Must point to a Scene
             */
            Scene* mScene;

            // List of all entities in this %SuperGameObject.
            std::vector<Entity*> mEntityVector;
    };
}
