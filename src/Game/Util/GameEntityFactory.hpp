#pragma once

class Scene;
class EnemyObject;


class GameObjectFactory {
    public:
        /// Get the instance of the class.
        /**
         * @return The %GameEntityFactory instance
         */
        static GameObjectFactory& GetInstance();
        
        ///Default constructor
        GameObjectFactory::GameObjectFactory();

        ///Constructor
        /**
         *@param scene The scene that the factory will be coupled to.
         */
        GameObjectFactory::GameObjectFactory(Scene* scene);
        
        ///Create a basic enemy.
        /**
         *@param origin The enemy's starting position
         */
        EnemyObject* GameObjectFactory::CreateBasicEnemy(glm::vec3 origin);

        ///Sets scene that the factory is coupled to.
        /**
         *@param scene Scene that the factory will be coupled to.
         */
        void SetScene(Scene* scene);

        ///Destructor
        GameObjectFactory::~GameObjectFactory();

    private:
        Scene* mScene;
};

/// Get the collision manager.
/**
 * @return The %CollisionManager instance
 */
GameObjectFactory& GameEntityCreator();