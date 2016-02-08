#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A Cave
    class Cave : public SuperGameObject {
        public:
            /// Create %Cave
            /**
             * @param scene Pointer to which Scene %Cave Enities' are contained.
             */
            Cave(Scene* scene);
           
            ~Cave();

            /// Map of all walls in the cave system.
            static const float mMap[25][25];
    };
}
