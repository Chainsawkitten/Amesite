#pragma once

#include <glm\glm.hpp>

#include <Util\Input.hpp>

class Scene;
class Entity;

namespace Caves{
    class CaveSystem;
}


class GameEntityFactory {
    public:
        /// Get the instance of the class.
        /**
         * @return The %GameEntityFactory instance
         */
        static GameEntityFactory& GetInstance();
        
        /// Default constructor
        GameEntityFactory();

        /// Constructor
        /**
         *@param scene The scene that the factory will be coupled to.
         */
        GameEntityFactory(Scene* scene);
        
        /// Destructor.
        ~GameEntityFactory();
        
        /// Create a basic enemy.
        /**
         *@param origin The enemy's starting position
         */
        Entity* CreateBasicEnemy(glm::vec3 origin);
       
        /// Create a player.
        /**
        *@param origin The enemy's starting position
        */
        Entity* GameEntityFactory::CreatePlayer(glm::vec3 origin, InputHandler::Player player);

		/// Create a cube for testing purposes.
        /**
        *@param origin The cube's starting position
        */
		Entity* CreateCube(glm::vec3 origin);

        /// Create a camera.
        /**
         *@param origin The camera's starting position
         *@param rotation The camera's starting orientation
         */
        Entity* CreateCamera(glm::vec3 origin, glm::vec3 rotation);

        ///Sets scene that the factory is coupled to.
        /**
         *@param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

        Caves::CaveSystem* CreateMap();

    private:
        Scene* mScene;
};

/// Get the collision manager.
/**
 * @return The %CollisionManager instance
 */
GameEntityFactory& GameEntityCreator();
