#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A SuperEnemy
    class SuperEnemy : public SuperGameObject {
        public:
            /// Create %SuperEnemy
            /**
             * @param scene Pointer to which Scene %SuperEnemy Enities' are contained.
             */
            SuperEnemy(Scene* scene);
           
            virtual ~SuperEnemy();

            /// Node of %SuperEnemy
            Entity* node;

            /// Returns true if the enemy is active.
            /**
             * @return State of the enemy (active or not).
             */
            bool Active();

            /// Activates the enemy.
            virtual void Activate();

            /// Deactivates the enemy.
            virtual void Deactivate();

            /// Gets health of the enemy.
            /**
             * @return health of the enemy.
             */
            virtual float GetHealth() = 0;

            /// How close the players have to come to active the enemy.
            /**
             * Default: 30.f
             */
            float range;

        protected:
            // Default false
            bool mActive;

            virtual void mUpdateFunction();
    };
}
