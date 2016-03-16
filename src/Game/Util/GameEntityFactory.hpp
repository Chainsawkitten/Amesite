#pragma once

#include <glm/glm.hpp>
#include <Util/Input.hpp>

class Scene;
class Entity;

namespace CaveGenerator {
    class Coordinate;
}

namespace Component {
    class Explode;
}

namespace GameObject{
    class Bullet;
    class SuperPlayer;
    class Player1;
    class Player2;
    class Camera;
    class Rocket;
    class Nest;
    class Pylon;
    class Cave;
    class Dust;
    class Explosion;
    class Shield;
    class SpinBoss;
    class ShieldBoss;
    class DivideBoss;
    class RingBoss;
    class EnemySpawner;
    class Altar;
    class Pillar;
    class PillarBall;
    class Portal;
    class Scenery;
    class ReviveCircle;
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
         * @return The created enemy spawner GameObject.
         */
        GameObject::EnemySpawner* CreateEnemySpawner(unsigned int type, float delay);
        
        /// Create a rocket enemy.
        /**
         * @param origin The enemy's starting position
         * @return The created enemy GameObject.
         */
        GameObject::Rocket* CreateRocket(const glm::vec3& origin);

        /// Create a mini rocket enemy.
        /**
         * @param origin The enemy's starting position
         * @return The created enemy GameObject.
         */
        GameObject::Rocket* CreateMiniRocket(const glm::vec3& origin);

        /// Create a nest enemy.
        /**
          * @param origin The enemy's starting position
          * @return The created enemy GameObject.
          */
        GameObject::Nest* CreateNest(const glm::vec3& origin);

        /// Create pylon enemy.
        /**
         * @param origin The enemy's starting position
         * @return The created pylon GameObject.
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

        /// Create player1.
        /**
         * @param origin The player's starting position.
         * @return The created player 1 GameObject.
         */
        GameObject::Player1* CreatePlayer1(const glm::vec3& origin);

        /// Create player2.
        /**
        * @param origin The player's starting position.
        * @return The created player 2 GameObject.
        */
        GameObject::Player2* CreatePlayer2(const glm::vec3& origin);

        /// Create SpinBoss.
        /**
         * @param origin The SpinBoss's starting position.
         * @return The created SpinBoss GameObject.
         */
        GameObject::SpinBoss* CreateSpinBoss(const glm::vec3& origin);

        /// Create ShieldBoss.
        /**
         * @param origin The ShieldBoss's starting position.
         * @return The created ShieldBoss GameObject.
         */
        GameObject::ShieldBoss* CreateShieldBoss(const glm::vec3& origin);

        /// Create DivideBoss.
        /**
         * @param origin The DivideBoss's starting position.
         * @return The created DivideBoss GameObject.
         */
        GameObject::DivideBoss* CreateDivideBoss(const glm::vec3& origin);

        /// Create RingBoss.
        /**
         * @param origin The RingBoss's starting position.
         * @return The created RingBoss GameObject.
         */
        GameObject::RingBoss* CreateRingBoss(const glm::vec3& origin);
        
        /// Create Altar.
        /**
         * @param origin The Altar's starting position.
         * @return The created Spawn GameObject.
         */
        GameObject::Altar* CreateAltar(const glm::vec3& origin);

        /// Create Pillar.
        /**
         * @param origin The Pillar's starting position.
         * @param bossPosition The Boss position.
         * @return The created Pillar GameObject.
         */
        GameObject::Pillar* CreatePillar(const glm::vec3& origin, glm::vec3 bossPosition);

        /// Create PillarBall.
        /**
         * @param origin The PillarBall's starting position.
         * @param destination The destination to point to.
         * @return The created PillarBall GameObject.
         */
        GameObject::PillarBall* CreatePillarBall(const glm::vec3& origin, const glm::vec3& destination);

        /// Create Portal.
        /**
         * @param origin The Portal's starting position.
         * @return The created Portal GameObject.
         */
        GameObject::Portal* CreatePortal(const glm::vec3& origin);

        /// Create crash site.
        /**
         * @return The crash entity.
         */
        Entity* CreateCrashSite1();

        /// Create crash site.
        /**
         * @return The created crash entity.
         */
        Entity* CreateCrashSite2();

        /// Create stone.
        /**
         * @return The created stone entity.
         */
        Entity* CreateStone();

        /// Create glowing stone.
        /**
         * @return The stone entity.
         */
        Entity* CreateGlowingStone();

        /// Create crystal light.
        /**
         * @return The created crystal entity.
         */
        Entity* CreateCrystalLight();

        /// Create fallen pillar.
        /**
         * @return The created pillar entity.
         */
        Entity* CreateFallenPillar();

        /// Create shrapnel.
        /**
         * @param position The Shrapnel's starting position.
         * @param amount The amount of shrapnel.
         * @param explodeComponent Explode component to give sharpnel.
         * @return The created shrapnel entity.
         */
        Entity* CreateShrapnel(glm::vec3 position, unsigned int amount, Component::Explode* explodeComponent = nullptr);

        /// Create player bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @param faction The faction, bullets only harm entities of other factions.
         * @return The created bullet GameObject.
         */
        GameObject::Bullet* CreatePlayerBullet(const glm::vec3& position, const glm::vec3& direction, int faction);

        /// Create enemy bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @param faction The faction, bullets only harm entities of other factions.
         * @return The created bullet GameObject.
         */
        GameObject::Bullet* CreateEnemyBullet(const glm::vec3& position, const glm::vec3& direction, int faction);

        /// Create a camera.
        /**
         * @param origin The camera's starting position.
         * @param rotation The camera's starting orientation.
         * @return The created camera GameObject.
         */
        GameObject::Camera* CreateCamera(const glm::vec3& origin, const glm::vec3& rotation);

        /// Creates a cuboid dust particle emitter and bind it to an Entity.
        /**
         * @param object Entity to which the system is relative.
         * @param particleTextureIndex %Texture the particles should sample from, these are documented in Component::ParticleEmitter::ParticleTextureIndex.
         * @return The created dust particle emitter.
         */
        GameObject::Dust* CreateDust(Entity* object, int particleTextureIndex);

        /// Creates a point fire particle emitter and bind it to an Entity.
        /**
         * @param position Position of the explosion.
         * @param lifeTime Life time of the explosion.
         * @param size Size of the explosion.
         * @param particleTextureIndex %Texture the particles should sample from, these are documented in Component::ParticleEmitter::ParticleTextureIndex.
         * @param explodeComponent Explode component to give shrapnel.
         * @return The created explosion.
         */
        GameObject::Explosion* CreateExplosion(glm::vec3 position, float lifeTime, float size, int particleTextureIndex, Component::Explode* explodeComponent = nullptr);

        /// Creates a ReviveCircle.
        /**
         * @param player Player which this reive circle corresponds to.
         * @return The created revive circle game object.
         */
        GameObject::ReviveCircle* CreateReviveCircle(GameObject::SuperPlayer* player);

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
