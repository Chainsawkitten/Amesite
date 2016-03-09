#pragma once

#include "SuperBoss.hpp"

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A RingBoss
    class RingBoss : public SuperBoss {
        public:
            /// Create %RingBoss
            /**
             * @param scene Pointer to which Scene %RingBoss Enities' are contained.
             */
            RingBoss(Scene* scene);
           
            ~RingBoss();

            /// Gets position of boss.
            /**
             * @return Position of the boss.
             */
            glm::vec3 GetPosition();

            /// Activates the enemy.
            void Activate();

            /// Deactivates the enemy.
            void Deactivate();

            /// Gets health of the boss.
            /**
             * @return Health of the boss.
             */
            float GetHealth();

            /// Ring relative to node
            struct Ring {
                Entity* node = nullptr;
                Entity* midFront = nullptr;
                Entity* leftFront = nullptr;
                Entity* rightFront = nullptr;
                Entity* left = nullptr;
                Entity* right = nullptr;
                Entity* leftBack = nullptr;
                Entity* rightBack = nullptr;
            } ring;
        protected:
            void mUpdateFunction();
        private:
            Geometry::OBJModel* mBodyModel;
            Geometry::OBJModel* mRingModel;

            void CreateRing();

            void CreateRingPart(Entity* entity, glm::vec3 position);
    };
}
