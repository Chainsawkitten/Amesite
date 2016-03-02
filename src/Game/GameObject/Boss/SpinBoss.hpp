#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>
#include <vector>

class Scene;
class Entity;
class Texture2D;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A SpinBoss
    class SpinBoss : public SuperGameObject {
        public:
            /// Create %SpinBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            SpinBoss(Scene* scene);
           
            ~SpinBoss();

            /// Node of %SpinBoss
            Entity* node;

            /// Body relative to node
            Entity* body;

            /// Arms relative to body
            Entity* armArr[4];

            /// Update boss.
            void Update();

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

            Geometry::OBJModel* mBody;
    };
}
