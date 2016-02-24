#pragma once
#include <glm\glm.hpp>
#include <vector>

namespace GameObject {
    class Player;
}

namespace System {
    ///Class that handles respawning players and manipulating checkpoint
    class CheckpointSystem {
        public:
            ///Checks if all players are dead. If they are, respawn them.
            void CheckIfPlayersAreDead();

            ///Moves checkpoint to designated location.
            /**
            * @param position New position for the checkpoint.
            */
            void MoveCheckpoint(glm::vec2 position);

            ///Adds player to mPlayers. 
            /**
            * @param player Player to add.
            */
            void AddPlayer(GameObject::Player* player);

        private:

            void RespawnPlayers();

            std::vector<GameObject::Player*> mPlayers;
            glm::vec2 mPosition;

    };
}