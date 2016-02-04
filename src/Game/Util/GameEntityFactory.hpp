#pragma once

#include <glm/glm.hpp>
#include <Util/Input.hpp>

class Scene;
class Entity;

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
         *@param origin The enemy's starting position
         */
        Entity* CreateBasicEnemy(glm::vec3 origin);
       
        /// Create a player.
        /**
         *@param origin The player's starting position.
         *@param player Who controls the player.
         */
        Entity* CreatePlayer(glm::vec3 origin, InputHandler::Player player);

		/// Create a cube for testing purposes.
        /**
         *@param origin The cube's starting position
         */
		Entity* CreateCube(glm::vec3 origin);
        
        /// Create a bullet.
        /**
         * @param position The bullet's starting position.
         * @param direction The direction the bullet is traveling in.
         * @return The bullet entity.
         */
        Entity* CreateBullet(glm::vec3 position, glm::vec3 direction);

        /// Create a camera.
        /**
         *@param origin The camera's starting position.
         *@param rotation The camera's starting orientation.
         */
        Entity* CreateCamera(glm::vec3 origin, glm::vec3 rotation);

        /// Sets scene that the factory is coupled to.
        /**
         *@param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

         /// Create a map.
        Entity* CreateMap();

    private:
        Scene* mScene;
};

/// Get the collision manager.
/**
 * @return The %CollisionManager instance
 */
GameEntityFactory& GameEntityCreator();
