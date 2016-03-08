#pragma once

#include "SuperEnemy.hpp"

class Scene;
class Entity;

class Texture2D;

namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A Rocket
    class Rocket : public SuperEnemy {
        public:
            /// Create %Rocket
            /**
             * @param scene Pointer to which Scene %Rocket Enities' are contained.
             */
            Rocket(Scene* scene);
           
            ~Rocket();

            /// Body relative to node
            Entity* body;

            /// Activates the enemy.
            void Activate();

            /// Deactivates the enemy.
            void Deactivate();

            /// Gets health of the enemy.
            /**
             * @return health of the enemy.
             */
            float GetHealth();
        protected:
            void mUpdateFunction();
        private:
            Geometry::OBJModel* mBodyModel;

            Texture2D* mActiveGlow;
            Texture2D* mDeactiveGlow;
    };
}
