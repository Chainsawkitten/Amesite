#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace CaveGenerator {
    class CaveMap;
}

namespace GameObject {
    /// A Cave
    class Cave : public SuperGameObject {
        public:
            /// Create %Cave
            /**
             * @param scene Pointer to which Scene %Cave Enities' are contained.
             * @param width Map width.
             * @param height Map height.
             * @param seed The seed used for random generation.
             * @param percent The amount of walls to place initially.
             * @param iterations How many iterations of smoothing we will do.
             * @param threshold The minimum room size in tiles.
             */
            Cave(Scene* scene, int width, int height, int seed, int percent, int iterations, int threshold);

            void UpdateMap();

            void CreateGeometry(Scene* scene);
            /// Destructor
            ~Cave();

            /// Map of all walls in the cave system.
            static bool** mMap;

            /// Map of the %Cave
            Entity* map;
            /// CaveMap instance.
            CaveGenerator::CaveMap* caveMap;

            float xScale;
            float zScale;
        private:
            int mWidth;
            int mHeight;
    };
}
