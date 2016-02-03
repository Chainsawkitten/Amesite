#pragma once

#include <vector>

class Scene;
class Entity;

namespace Component {
    class Transform;
    class Mesh;
}

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
            Entity* GenerateCaveSystem();
            
        private:
            Scene* mScene;
            
            static const float mMap[25][25];
    };
}
