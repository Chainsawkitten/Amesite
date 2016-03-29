#pragma once

#include <GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

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

            /// Startposition of the ball.
            /**
             * Default: 0.f, 0.f
             */
            glm::vec2 startPosition;

            /// startHeight of the ball.
            /**
             * Default: 0.f
             */
            float startHeight;

            /// maxHeight of the ball.
            /**
             * Default: 10.f
             */
            float maxHeight;

            /// Destination of the ball
            /**
             * Default: 1.f, 0.f
             */
            glm::vec2 destination;

        private:
            Geometry::OBJModel* mBallModel;

            void AddPartilces(Entity* entity);

            void mUpdateFunction();
    };
}
