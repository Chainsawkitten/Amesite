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
            
            /// Did we respawn this frame
            bool mRespawn;

            // Times the players fucked up and wiped.
            int timesDied;
        private:
            Audio::SoundBuffer* mLowHPSoundBuffer;
            bool mPlayingBeepSound;
            void RespawnPlayers();
            glm::vec2 mPosition;
            ALuint mBeepSource;
    };
}