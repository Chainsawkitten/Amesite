#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

namespace GameObject {
    /// A SuperPlayer
    class SuperPlayer : public SuperGameObject {
        public:
            /// Create %SuperPlayer
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             * @param id Id of the player.
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
             * @param Position of the player.
             */
            virtual void SetPosition(glm::vec3 position) = 0;

            /// Gets node Entity
            /**
             * @return Main Entity contaning tranform and physics components.
             */
            virtual Entity* GetNodeEntity() = 0;

            /// Gets health of the player.
            /**
             * @return Health of the player.
             */
            virtual float GetHealth() = 0;

            /// Update player.
            virtual void Update() = 0;

            /// Returns true if the player is active.
            bool Active();

            /// Activates the player.
            virtual void Activate() = 0;

            /// Deactivates the player.
            virtual void Deactivate() = 0;

            /// How long the player has been respawning
            float mRespawnTimer;

        protected:
            //The players current state.
            enum PlayerState {
                LIGHTDAMAGE = 0,
                MEDIUMDAMAGE,
                HEAVYDAMAGE
            } mState;

            bool mActive;
    };
}
