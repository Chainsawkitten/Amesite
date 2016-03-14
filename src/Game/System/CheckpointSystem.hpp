#pragma once
#include <glm\glm.hpp>
#include <vector>

namespace GameObject {
    class SuperPlayer;
}

namespace System {
    ///Class that handles respawning players and manipulating checkpoint
    class CheckpointSystem {
        public:

            CheckpointSystem();

            ///Checks if all players are dead. If they are, respawn them.
            /**
             * @param deltaTime time since last frame.
             */
            void Update(float deltaTime);

            ///Moves checkpoint to designated location.
            /**
             * @param position New position for the checkpoint.
             */
            void MoveCheckpoint(glm::vec2 position);

            ///Adds player to mPlayers. 
            /**
             * @param player Player to add.
             */
            void AddPlayer(GameObject::SuperPlayer* player);
            
            /// Did we respawn this frame
            bool mRespawn;

        private:
            void RespawnPlayers();
            std::vector<GameObject::SuperPlayer*> mPlayers;
            glm::vec2 mPosition;

    };
}