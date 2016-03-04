#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// PillarBall
    class PillarBall : public SuperGameObject {
        public:
            /// Create %PillarBall
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            PillarBall(Scene* scene);
           
            ~PillarBall();

            /// Node of %PillarBall
            Entity* node;

            /// Ball1 relative to Node
            Entity* ball1;

            /// Ball2 relative to Node
            Entity* ball2;

        private:
            Geometry::OBJModel* mBallModel;

            void AddPartilces(Entity* entity);

            void mUpdateFunction();
    };
}
