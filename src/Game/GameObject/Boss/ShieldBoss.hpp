#pragma once

#include "SuperBoss.hpp"

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A ShieldBoss
    class ShieldBoss : public SuperBoss {
        public:
            /// Create %ShieldBoss
            /**
             * @param scene Pointer to which Scene %ShieldBoss Enities' are contained.
             */
            ShieldBoss(Scene* scene);
           
            ~ShieldBoss();

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

            struct Cover {
                Entity* node = nullptr;
                Entity* leftShield = nullptr;
                Entity* rightShield = nullptr;
            };

            /// Cover relative to node
            Cover coverArr[3];
        protected:
            void mUpdateFunction();
        private:
            Geometry::OBJModel* mBodyModel;
            Geometry::OBJModel* mCoverModel;

            void CreateCover(Cover* cover);
            
            void FireBullets();

            float mMaxSpawnerDelay;
            int mOffsetAngle;
    };
}
