#pragma once

#include "SuperBoss.hpp"

#include <glm/glm.hpp>

class Scene;
class Entity;
class Texture2D;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A SpinBoss
    class SpinBoss : public SuperBoss {
        public:
            /// Create %SpinBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            SpinBoss(Scene* scene);
           
            ~SpinBoss();

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

            /// Arms relative to body
            Entity* armArr[6];
        protected:
            void mUpdateFunction();
        private:
            //The boss current state.
            enum BossState {
                REFLECT = 0,
                NOREFLECT,
            } mState;

            //The boss current phase.
            enum BossPhase {
                ONE = 0,
                TWO,
            } mPhase;

            Texture2D* mNoReflectTexture;
            Texture2D* mReflectTexture;

            int mNrOfArms;
            float mCurrentSpawnerDelay;
            float mBulletSize;

            void CreateArm(Entity* entity);

            void ChangePhase(BossPhase phase);

            void FireBullet(glm::vec3 bossPos, glm::vec3 armPos);

            Geometry::OBJModel* mBody;
    };
}
