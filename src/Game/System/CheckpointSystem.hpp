#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <AL/al.h>

namespace GameObject {
    class SuperPlayer;
}

namespace Audio {
    class SoundBuffer;
}

namespace System {
    ///Class that handles respawning players and manipulating checkpoint
    class CheckpointSystem {
        public:
            ///Constructor
            CheckpointSystem();

            ///Destructor
            ~CheckpointSystem();

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

        private:
            Audio::SoundBuffer* mLowHPSoundBuffer;
            bool mPlayingBeepSound;
            void RespawnPlayers();
            std::vector<GameObject::SuperPlayer*> mPlayers;
            glm::vec2 mPosition;
            ALuint mBeepSource;
    };
}