#pragma once

class Scene;

namespace Component {
    class Collider2DCircle;
    //class Collider2DRectangle;
}

namespace System {
    /// Class providing collision detection.
    class CollisionSystem {
        public:
            /// Create %CollisionSystem.
            CollisionSystem();

            /// Destructor.
            ~CollisionSystem();

            /// Update collision vector.
            void Update(Scene& scene);

        private:

            /// Check rectangle vs rectangle collision.
            /**
             *@param aRectangle First entity with rectangle component.
             *@param bRectangle Second entity with rectangle component.
             *@return whether collision occured between a & b
             */
            //bool RectangleVSRectangle(Component::Collider2DRectangle* aRectangle, Component::Collider2DRectangle* bRectangle);

            /// Check rectangle vs circle collision.
            /**
             *@param aRectangle First entity with rectangle component.
             *@param bCircle Second entity with circle component.
             *@return whether collision occured between a & b
             */
            //bool RectangleVSCircle(Component::Collider2DRectangle* aRectangle, Component::Collider2DCircle* bCircle);

            /// Check circle vs circle collision.
            /**
             *@param aCircle First entity with circle component.
             *@param bCircle Second entity with circle component.
             *@return whether collision occured between a & b
             */
            bool CircleVSCircle(Component::Collider2DCircle* aCircle, Component::Collider2DCircle* bCircle);
        };


}
