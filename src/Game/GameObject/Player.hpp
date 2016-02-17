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
            Entity* frontEngineLeft;

            /// Engine relative to body.
            Entity* frontEngineRight;

            /// Engine relative to body.
            Entity* backEngineLeft;

            /// Engine relative to body.
            Entity* backEngineRight;

            /// Light relative to body.
            Entity* light;

            /// Turrent relative to body.
            Entity* leftTurrent;

            /// Turrent relative to body.
            Entity* rightTurrent;

        private:
            Geometry::OBJModel* mShipBody;
            Geometry::OBJModel* mShipFrontEngineRight;
            Geometry::OBJModel* mShipFrontEngineLeft;
            Geometry::OBJModel* mShipBackEngineRight;
            Geometry::OBJModel* mShipBackEngineLeft;

            void AddEnginePartilces(Entity* entity);
    };
}
