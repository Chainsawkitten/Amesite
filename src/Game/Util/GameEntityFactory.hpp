#pragma once

#include <glm\glm.hpp>

class Scene;
class Entity;


class GameEntityFactory {
    public:
        /// Get the instance of the class.
        /**
         * @return The %GameEntityFactory instance
         */
        static GameEntityFactory& GetInstance();
        
        ///Default constructor
        GameEntityFactory::GameEntityFactory();

        ///Constructor
        /**
         *@param scene The scene that the factory will be coupled to.
         */
        GameEntityFactory::GameEntityFactory(Scene* scene);
        
        ///Create a basic enemy.
        /**
         *@param origin The enemy's starting position
         */
        Entity* GameEntityFactory::CreateBasicEnemy(glm::vec3 origin);

        ///Create a camera.
        /**
        *@param origin The enemy's starting position
        */
        Entity* GameEntityFactory::CreateCamera(glm::vec3 origin, glm::vec3 rotation);

        ///Sets scene that the factory is coupled to.
        /**
         *@param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

        ///Destructor
        GameEntityFactory::~GameEntityFactory();

    private:
        Scene* mScene;
};

/// Get the collision manager.
/**
 * @return The %CollisionManager instance
 */
GameEntityFactory& GameEntityCreator();