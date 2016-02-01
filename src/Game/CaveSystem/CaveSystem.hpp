#pragma once
#include <Entity/Entity.hpp>
#include <vector>
#include <Scene/scene.hpp>

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
            void GenerateCaveSystem();
            
        private:
            Scene* mScene;

            static const float mMap[25][25];
    };
}
