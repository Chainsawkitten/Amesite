#pragma once

class Entity;

namespace System {
    ///Class providing collision detection
    class CollisionSystem {
        public:
            /// New %CollisionSystem.
            CollisionSystem();

            ~CollisionSystem();

            /// Check rectangle vs rectangle collision
            /**
             *@param aRectangle First entity with rectangle component.
             *@param bRectangle Second entity with rectangle component.
             *@return whether collision occured between a & b
             */
            bool RectangleVSRectangle(Entity* aRectangle, Entity* bRectangle);

            /// Check rectangle vs circle collision
            /**
             *@param aRectangle First entity with rectangle component.
             *@param bCircle Second entity with circle component.
             *@return whether collision occured between a & b
             */
            bool RectangleVSCircle(Entity* aRectangle, Entity* bCircle);

            /// Check circle vs circle collision
            /**
             *@param aCircle First entity with circle component.
             *@param bCircle Second entity with circle component.
             *@return whether collision occured between a & b
             */
            bool CircleVSCircle(Entity* aCircle, Entity* bCircle);
        };
}
