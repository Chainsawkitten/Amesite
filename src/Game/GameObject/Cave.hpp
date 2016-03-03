#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
namespace CaveGenerator {
    class CaveMap;
    class Coordinate;
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
             * @param playerPosition Where the players start.
             * @param bossPositions A vector of boss positions.
             */
            Cave(Scene* scene, int width, int height, int seed, int percent, int iterations, int threshold, CaveGenerator::Coordinate playerPosition, std::vector<CaveGenerator::Coordinate> bossPositions );

            /// Destructor
            ~Cave();

            /// Gets width of the cave.
            /**
             * @return Width of the cave.
             */
            int GetWidth() const;

            /// Gets height of the cave.
            /**
             * @return Height of the cave.
             */
            int GetHeight() const;

            /// Gets radius of the boss room.
            /**
            * @return Radius of the boss room.
            */
            int GetBossRoomRadius() const;

            /// Grid collision
            /**
             * @param entity The minimum room size in tiles.
             * @param deltaTime Where the players start.
             * @return Returns true if collision with map, else false.
             */
            bool GridCollide(Entity* entity, float deltaTime);

            /// Gets bool grid of cave.
            /**
             * @return 2D bool array of the cave grid.
             */
            bool** GetCaveData() const;

            /// Map of all walls in the cave system.
            bool** mMap;

            /// Map of the %Cave
            Entity* map;
            Entity* heightMap;

            /// CaveMap instance.
            CaveGenerator::CaveMap* caveMap;

            /// Scale factor.
            /**
             * Default: 5.f
             */
            float scaleFactor;

        private:

            int PointCollide(glm::vec3 point, glm::vec3 velocity, float deltaTime);

            /// cave X size.
            int mWidth;

            /// cave Z size.
            int mHeight;

            /// radius of a boss room.
            int mBossRadius;
    };
}
