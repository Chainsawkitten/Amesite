#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>
#include "../../Component/Controller.hpp"

#include <glm/glm.hpp>

namespace GameObject {
    /// A SuperPlayer
    class SuperPlayer : public SuperGameObject {
        protected:
            ///Stores whether the player is active or not.
            bool mActive;

        public:
            ///The players current state.
            enum PlayerState {
                LIGHTDAMAGE = 0,     ///Player lightly damaged.
                MEDIUMDAMAGE,        ///Player mediumly damaged.
                HEAVYDAMAGE,         ///Player heavily damaged.
                DEAD                 ///Player dead.
            };
            
            ///Contains the players current state.
            PlayerState mState;

            /// Create %SuperPlayer
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            SuperPlayer(Scene* scene);
           
            virtual ~SuperPlayer();

            /// Gets position of player
            /**
             * @return Position of the player.
             */
            virtual glm::vec3 GetPosition() = 0;

            /// Sets position of player
            /**
             * @param position Position of the player.
             */
            virtual void SetPosition(glm::vec3 position) = 0;

            /// Gets node Entity
            /**
             * @return Main Entity contaning tranform and physics components.
             */
            virtual Entity* GetNodeEntity() = 0;
            
            /// Gets body entity.
            /**
             * This is the entity containing the player's main mesh and is different from the node entity as the animation should not affect collision deteciton.
             * @return The entity containing the player's main mesh.
             */
            virtual Entity* GetBodyEntity() = 0;

            /// Gets health of the player.
            /**
             * @return Health of the player.
             */
            virtual float GetHealth() = 0;

            /// Returns true if the player is active.
            /**
             * @return State of the player (active or not).
             */
            bool Active();

            /// Set device
            /**
             * @param Device to control player
             */
            virtual void SetDevice(InputHandler::Device device) = 0;

            /// Activates the player.
            virtual void Activate() = 0;

            /// Deactivates the player.
            virtual void Deactivate() = 0;

            /// Rotates player.
            /**
             * @param yaw Angle to rotate player.
             */
            virtual void SetYaw(float yaw) = 0;

            /// The initial time the player has to been respawned (in seconds).
            /**
             * Default: 5.f
             */
            float initalRespawnTime;

            /// How long time the player has left to be respawned (in seconds).
            float respawnTimeLeft;
            /**
             * Default: initalRespawnTime (5.f)
             */
    };
}
