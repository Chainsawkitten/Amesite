#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;

namespace GameObject {
    /// A SuperBoss
    class SuperBoss : public SuperGameObject {
        public:
            /// Create %SuperBoss
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            SuperBoss(Scene* scene);
           
            virtual ~SuperBoss();

            /// Gets position of player
            /**
             * @return Position of the player.
             */
            virtual glm::vec3 GetPosition() = 0;

            /// Gets health of the player.
            /**
             * @return Health of the player.
             */
            virtual float GetHealth() = 0;

            /// Node of %SuperBoss, contains health and transform
            Entity* node;

            /// Body relative to node
            Entity* body;
    };
}
