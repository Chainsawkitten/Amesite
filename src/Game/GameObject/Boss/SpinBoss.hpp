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

            /// Arms relative to body
            Entity* armArr[4];

            /// Gets health of the boss.
            /**
             * @return Health of the boss.
             */
            float GetHealth();

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

            void CreateArm(Entity* entity, glm::vec3 direction);

            void ChangePhase(BossPhase phase);

            void mUpdateFunction();

            Geometry::OBJModel* mBody;
    };
}
