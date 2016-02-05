#pragma once

class Entity;
class Scene;

#include <vector>

namespace GameObject {
    /// Game object containing a vector of entites which all game objects generalize from.
    class SuperGameObject {
        public:
            /// Create a %SuperGameObject
            SuperGameObject(Scene* scene);

            virtual ~SuperGameObject();

            void Clear();

        protected:
            Scene* mScene;

            std::vector<Entity*> mEntityVector;
    };
}
