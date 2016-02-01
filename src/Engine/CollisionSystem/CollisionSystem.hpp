#pragma once
#include "../Entity/Entity.hpp"
#include "../Component/Collider2DCircle.hpp"
#include "../Component/Collider2DRectangle.hpp"

class CollisionSystem {
    public:
        /// Get the instance of the class.
        /**
        * @return The %CollisionSystem instance
        */
        static CollisionSystem& GetInstance();

        /// Check rectangle vs rectangle collision
        /**
        *@param aRectangle First entity with rectangle component.
        *@param bRectangle Second entity with rectangle component.
        *@return whether collision occured between a & b
        */
        bool RectangleVSRectangle(Entity *aRectangle, Entity *bRectangle);

        /// Check rectangle vs circle collision
        /**
        *@param aRectangle First entity with rectangle component.
        *@param bCircle Second entity with circle component.
        *@return whether collision occured between a & b
        */
        bool RectangleVSCircle(Entity *aRectangle, Entity *bCircle);

        /// Check circle vs circle collision
        /**
        *@param aCircle First entity with circle component.
        *@param bCircle Second entity with circle component.
        *@return whether collision occured between a & b
        */
        bool CircleVSCircle(Entity *aCircle, Entity *bCircle);
};

/// Get the collision manager.
/**
* @return The %CollisionManager instance
*/
CollisionSystem& CollisionManager();