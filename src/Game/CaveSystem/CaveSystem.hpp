#pragma once
#include <Entity/Entity.hpp>
#include <vector>
#include <Scene/scene.hpp>

namespace Component {
    class Transform;
    class Mesh;
}

namespace Caves {
    class CaveSystem {
        public:
            ///Constructor
            /**
            *@Param scene pointer to the active scene.
            */
            CaveSystem(Scene* scene);
            ~CaveSystem();

            ///Creates a cavesystem from map out of cubes
            void GenerateCaveSystem();
        private:
            Scene* mScene;

            static const float mMap[25][25];
    };
}
