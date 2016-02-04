#pragma once

#include <vector>

class Scene;
class Entity;

namespace Caves {
    /// Generates caves for the player to explore.
    class CaveSystem {
        public:
            /// Create new cave system.
            /**
             * @param scene Pointer to the active scene.
             */
            CaveSystem();
            
            /// Destructor.
            ~CaveSystem();

            /// Creates a cave system from map out of cubes.
            Entity* GenerateCaveSystem(Scene* scene);
            
        private:
            static const float mMap[25][25];
    };
}
