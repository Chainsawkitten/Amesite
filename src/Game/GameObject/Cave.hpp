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
            Cave(Scene* scene, int width, int height, int seed, int percent, int iterations, int threshold);
           
            ~Cave();


            /// Map of all walls in the cave system.
            static bool** mMap;

            /// Map of the %Cave
            Entity* map;
        private:
            int mWidth;
            int mHeight;
    };
}
