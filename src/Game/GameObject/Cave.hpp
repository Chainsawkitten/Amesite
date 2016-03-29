#pragma once

#include <GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
namespace CaveGenerator {
    class CaveMap;
    class Coordinate;
}

namespace Geometry {
    class Plane;
    class Terrain;
}

namespace GameObject {
    class Scenery;
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
             * @param The entity
             * @param deltaTime Where the players start.
             * @return Returns true if collision with map, else false.
             */
            bool GridCollide(Entity* entity, float deltaTime);

            /// Does it intersect with a wall
            /**
             * @param The entity
             * @param deltaTime Where the players start.
             * @return Returns true if collision with map, else false.
             */
            bool WallIntersect(Entity* entity, float deltaTime);

            /// Grid collision
            /**
             * @param point to check.
             * @return Returns true if collision with map, else false.
             */
            bool GridCollide(glm::vec3 point);

            /// Gets bool grid of cave.
            /**
             * @return 2D bool array of the cave grid.
             */
            bool** GetCaveData() const;

            /// Place an entity randomly into the world
            /**
             * @param scenery The entity to place
             * @param rotate Rotation of the entity to place
             */
            void PlaceScenery(Entity* scenery, bool rotate);

            /// Returns height value of the terrain
            /**
             * @param x The x-position in the world to check terrian height (world space)
             * @param z The z-position in the world to check terrian height (world space)
             * @return Returns y-position of terrian height (world space) 
             */
            float GetTerrainHeight(float x, float z);

            /// Map of all walls in the cave system.
            bool** mMap;
            
            /// Map that contains marching cube info.
            int** mTypeMap;

            /// Map of the %Cave
            Entity* map;
            Entity* heightMap;

            /// Rocks vector
            std::vector<Entity*> mSceneryVector;

            /// CaveMap instance.
            CaveGenerator::CaveMap* caveMap;

            /// Scale factor.
            /**
             * Default: 5.f
             */
            float scaleFactor;

        private:

            ///Takes a point and checks if it collides with a wall
            /**
             * @param The point too check for collision.
             * @param How far the point will move this frame
             * @param deltaTime Where the players start.
             * @return Returns true if it's inside a wall
             */
            glm::vec3 PointCollide(glm::vec3 point, glm::vec3 velocity, float deltaTime);

            ///Takes a position and checks it agains the marching square type to see if it's in a wall
            /**
             * @param the xPos (0 - 1) where 0 is far to the left and 1 is far to the right.
             * @param the yPos (0 - 1) where 0 is the top and 1 is the bottom.
             * @param the x coordinate of the cell
             * @param the y coordinate of the cell
             * @return Returns true if it's inside a wall
             */
            glm::vec3 CellCollide(float xPos, float yPos, int x, int y);
            
            /// cave X size.
            int mWidth;

            /// cave Z size.
            int mHeight;

            /// radius of a boss room.
            int mBossRadius;

            /// The border geometry
            Geometry::Plane* mBorder;

            /// The terrain geometry
            Geometry::Terrain* mTerrain;

            ///The four border entites
            Entity* mTopBorder;
            Entity* mBottomBorder;
            Entity* mRightBorder;
            Entity* mLeftBorder;

    };
}
