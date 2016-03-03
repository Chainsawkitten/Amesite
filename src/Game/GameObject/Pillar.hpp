#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// Pillar
    class Pillar : public SuperGameObject {
        public:
            /// Create %Pillar
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             * @param bossPosition Vector of boss position.
             */
            Pillar(Scene* scene, glm::vec3 bossPosition);
           
            ~Pillar();

            /// Node of %Pillar
            Entity* node;

            /// States
            enum State {
                ACTIVE = 0,
                DEACTIVATED,
            };

            /// SetState
            void SetState(State state);

            /// Gets state of the pillar.
            /**
             * @return State of the pillar.
             */
            State GetState();

        private:

            void mUpdateFunction();

            void CreateLight();

            Geometry::OBJModel* mPillarModel;
            Geometry::OBJModel* mLightModel;

            State mState;
            State mLastState;

            Entity* mLight;

            glm::vec3 mBossPosition;
    };
}
