#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;

namespace GameObject {
    /// A SuperBoss
    class SuperBoss : public SuperGameObject {
        public:
            /// Create %SuperBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            SuperBoss(Scene* scene);
           
            virtual ~SuperBoss();

            /// Gets position of boss
            /**
             * @return Position of the boss.
             */
            virtual glm::vec3 GetPosition() = 0;

            /// Returns true if the boss is active.
            /**
             * @return State of the boss (active or not).
             */
            bool Active();

            /// Activates the boss.
            virtual void Activate();

            /// Deactivates the boss.
            virtual void Deactivate();

            /// Gets health of the boss.
            /**
             * @return Health of the boss.
             */
            virtual float GetHealth() = 0;

            /// Node of %SuperBoss, contains health and transform
            Entity* node;

            /// Body relative to node
            Entity* body;

            /// How close the players have to come to active the boss.
            /**
             * Default: 80.f
             */
            float range;

        protected:
            // Default false
            bool mActive;

            virtual void mUpdateFunction();
    };
}
