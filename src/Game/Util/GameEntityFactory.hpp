#pragma once

#include <glm/glm.hpp>
#include <Util/Input.hpp>

class Scene;
class Entity;
class Texture2D;

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
         * @return The enemy entity.
         */
        Entity* CreateBasicEnemy(const glm::vec3& origin);
       
        /// Create a player.
        /**
         * @param origin The player's starting position.
         * @param player Who controls the player.
         * @return The player entity.
         */
        Entity* CreatePlayer(const glm::vec3& origin, InputHandler::Player player);

		/// Create a cube for testing purposes.
        /**
         * @param origin The cube's starting position
         * @return The cube entity.
         */
		Entity* CreateCube(const glm::vec3& origin);
        
        /// Create a bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @return The bullet entity.
         */
        Entity* CreateBullet(const glm::vec3& position, const glm::vec3& direction);

        /// Create a bullet.
        /**
        * @param position The bullet's starting position.
        * @param direction The direction the bullet is traveling in.
        * @return The bullet entity.
        */
        Entity* CreateEnemyBullet(const glm::vec3& position, const glm::vec3& direction);

        /// Create a camera.
        /**
         * @param origin The camera's starting position.
         * @param rotation The camera's starting orientation.
         * @return The camera entity.
         */
        Entity* CreateCamera(const glm::vec3& origin, const glm::vec3& rotation);

        /// Sets scene that the factory is coupled to.
        /**
         * @param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

        /// Creates a cuboid dust particle system.
        /**
         * @param camera - entity which the system relates to.
         * @param particleTexture the particles should sample from.
         */
        void CreateCuboidParticle(Entity* camera, Texture2D* particleTexture);

        /// Creates a cuboid dust particle system.
        /**
        * @param camera - entity which the system relates to.
        * @param particleTexture the particles should sample from.
        */
        void CreatePointParticle(Entity* object, Texture2D* particleTexture);

        /// Create a map.
        /**
         * @return The map entity.
         */ 
        Entity* CreateMap();

    private:
        Scene* mScene;
};

/// Get the collision manager.
/**
 * @return The %CollisionManager instance
 */
GameEntityFactory& GameEntityCreator();
