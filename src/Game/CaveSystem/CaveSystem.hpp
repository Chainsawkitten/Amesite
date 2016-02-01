#pragma once

#include <vector>

class Scene;

namespace Caves {
    /// Generates caves for the player to explore.
    class CaveSystem {
        public:
            /// Create new cave system.
            /**
             * @param scene Pointer to the active scene.
             */
            CaveSystem(Scene* scene);
            
            /// Destructor.
            ~CaveSystem();

            /// Creates a cave system from map out of cubes.
            void GenerateCaveSystem();
            
        private:
            Scene* mScene;

            static const float mMap[25][25];
    };
}
