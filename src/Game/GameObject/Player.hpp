#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A Player
    class Player : public SuperGameObject {
        public:
            /// Create %Player
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            Player(Scene* scene);
           
            ~Player();

            /// Gets position of player
            /**
             * @return Position of the player.
             */
            glm::vec3 GetPosition();

            /// Gets health of the player.
            /**
             * @return health of the player.
             */
            float GetHealth();

            /// Shoot a bullet.
            void Shoot();

            /// Position of the %Player.
            Entity* node;

            /// Body relative to node.
            Entity* body;

            /// Engine relative to body.
            Entity* leftFrontEngine;

            /// Engine relative to body.
            Entity* rightFrontEngine;

            /// Engine relative to body.
            Entity* leftBackEngine;

            /// Engine relative to body.
            Entity* rightBackEngine;

        private:
            Geometry::OBJModel* mShipBody;
            Geometry::OBJModel* mShipFrontEngine;
            Geometry::OBJModel* mShipBackEngine;
    };
}
