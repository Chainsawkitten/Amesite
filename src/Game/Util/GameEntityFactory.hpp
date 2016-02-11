#pragma once

#include <glm/glm.hpp>
#include <Util/Input.hpp>

class Scene;
class Entity;

namespace GameObject{
    class Bullet;
    class Player;
    class Camera;
    class Enemy;
    class Cave;
    class Dust;
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
        
        /// Create a basic enemy.
        /**
         * @param origin The enemy's starting position
         * @return The enemy GameObject.
         */
        GameObject::Enemy* CreateBasicEnemy(const glm::vec3& origin);
       
        /// Create a player.
        /**
         * @param origin The player's starting position.
         * @param player Who controls the player.
         * @return The player GameObject.
         */
        GameObject::Player* CreatePlayer(const glm::vec3& origin, InputHandler::Player player);
        
        /// Create a bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @param faction The faction, bullets only harm entites of other factions.
         * @return The bullet GameObject.
         */
        GameObject::Bullet* CreateBullet(const glm::vec3& position, const glm::vec3& direction, int faction);

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

        /// Sets scene that the factory is coupled to.
        /**
         * @param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

        /// Create a map.
        /**
         * @return The Map GameObject.
         */ 
        GameObject::Cave* CreateMap();

    private:
        Scene* mScene;
};

/// Get the game entity creator.
/**
 * @return The %GameEntityCreator instance
 */
GameEntityFactory& GameEntityCreator();
