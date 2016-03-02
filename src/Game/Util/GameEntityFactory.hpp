#pragma once

#include <glm/glm.hpp>
#include <Util/Input.hpp>

class Scene;
class Entity;

namespace CaveGenerator {
    class Coordinate;
}

namespace GameObject{
    class Bullet;
    class Player;
    class Camera;
    class Enemy;
    class Cave;
    class Dust;
    class Explosion;
    class Pylon;
    class Shield;
    class SpinBoss;
    class EnemySpawner;
}

/// Factory responsible for creating prefab entities.
class GameEntityFactory {
    public:
        /// Get the instance of the class.
        /**
         * @return The %GameEntityFactory instance
         */
        static GameEntityFactory& GetInstance();
        
        /// Default constructor
        GameEntityFactory();
        
        /// Destructor.
        ~GameEntityFactory();

        /// Create an enemy spawner.
        /**
         * @param type of enemy the spawner handles.
         * @param delay for spawning a new enemy.
         * @return The enemy spawner GameObject.
         */
        GameObject::EnemySpawner* CreateEnemySpawner(unsigned int type, float delay);
        
        /// Create a basic enemy.
        /**
         * @param origin The enemy's starting position
         * @return The enemy GameObject.
         */
        GameObject::Enemy* CreateBasicEnemy(const glm::vec3& origin);

        /// Create pylon enemy.
        /**
         * @param origin The enemy's starting position
         * @return The pylon GameObject.
         */
        GameObject::Pylon* CreateEnemyPylon(const glm::vec3& origin);
       
        /// Create shield.
        /**
         * @param parentEntity The shield's parent entity.
         * @param offset Offset of the shield to the parent entity.
         * @param lifeTime Life time of the shield.
         * @param health Health of the shield.
         * @return The created shield GameObject.
         */
        GameObject::Shield* CreateShield(Entity* parentEntity, glm::vec3 offset, float lifeTime, float health);

        /// Create a player.
        /**
         * @param origin The player's starting position.
         * @param player Who controls the player.
         * @return The player GameObject.
         */
        GameObject::Player* CreatePlayer(const glm::vec3& origin, InputHandler::Player player);

        /// Create SpinBoss.
        /**
         * @param origin The SpinBoss's starting position.
         * @return The SpinBoss GameObject.
         */
        GameObject::SpinBoss* CreateSpinBoss(const glm::vec3& origin);
        
        /// Create player bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @param faction The faction, bullets only harm entities of other factions.
         * @return The bullet GameObject.
         */
        GameObject::Bullet* CreatePlayerBullet(const glm::vec3& position, const glm::vec3& direction, int faction);

        /// Create enemy bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @param faction The faction, bullets only harm entities of other factions.
         * @return The bullet GameObject.
         */
        GameObject::Bullet* CreateEnemyBullet(const glm::vec3& position, const glm::vec3& direction, int faction);

        /// Create a camera.
        /**
         * @param origin The camera's starting position.
         * @param rotation The camera's starting orientation.
         * @return The camera GameObject.
         */
        GameObject::Camera* CreateCamera(const glm::vec3& origin, const glm::vec3& rotation);

        /// Creates a cuboid dust particle emitter and bind it to an Entity.
        /**
         * @param object Entity to which the system is relative.
         * @param particleTextureIndex %Texture the particles should sample from, these are documented in Component::ParticleEmitter::ParticleTextureIndex.
         */
        GameObject::Dust* CreateDust(Entity* object, int particleTextureIndex);

        /// Creates a point fire particle emitter and bind it to an Entity.
        /**
         * @param position Position of the explosion.
         * @param lifeTime Life time of the explosion.
         * @param size Size of the explosion.
         * @param particleTextureIndex %Texture the particles should sample from, these are documented in Component::ParticleEmitter::ParticleTextureIndex.
         */
        GameObject::Explosion* CreateExplosion(glm::vec3 position, float lifeTime, float size, int particleTextureIndex);

        /// Sets scene that the factory is coupled to.
        /**
         * @param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

        /// Create a map.
        /**
         * @param width Map width.
         * @param height Map height.
         * @param seed The seed used for random generation.
         * @param percent The amount of walls to place initially.
         * @param iterations How many iterations of smoothing we will do.
         * @param threshold The minimum room size in tiles.
         * @param playerPosition Where the players start.
         * @param bossPositions A vector of boss positions.
         * @return The Map GameObject.
         */ 
        GameObject::Cave* CreateMap(int width, int height, int seed, int percent, int iterations, int threshold, CaveGenerator::Coordinate playerPosition, std::vector<CaveGenerator::Coordinate> bossPositions);

    private:
        Scene* mScene;
};

/// Get the game entity creator.
/**
 * @return The %GameEntityCreator instance
 */
GameEntityFactory& GameEntityCreator();
