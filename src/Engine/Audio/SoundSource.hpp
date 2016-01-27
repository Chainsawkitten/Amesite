#pragma once

#include <glm/glm.hpp>
#include <AL/al.h>

namespace Audio {
    class SoundBuffer;
    
    /// A sound source in the game world.
    /**
     * Represents a sound source in the game world. If its SoundBuffer is in mono, the sound will be 3D and played relative to the Listener position. If the SoundBuffer is in stereo it will not be 3D.
     */
    class Sound {
        public:
            /// Create new sound source.
            /**
             * @param buffer The SoundBuffer containing the sound.
             */
            Sound(SoundBuffer* buffer);
            
            /// Destructor
            ~Sound();
            
            /// Get position.
            /**
             * Starting position: (0.0, 0.0, 0.0).
             * @return The sound source's position in the game world
             */
            const glm::vec3& Position() const;
            
            /// Set position.
            /**
             * @param position New position for the sound source.
             */
            void SetPosition(const glm::vec3& position);
            
            /// Get pitch.
            /**
             * Default 1.0.
             * @return The sound source's pitch
             */
            ALfloat Pitch() const;
            
            /// Set pitch.
            /**
             * @param pitch The sound source's pitch.
             */
            void SetPitch(ALfloat pitch);
            
            /// Get gain.
            /**
             * Default 1.0.
             * @return The sound's gain.
             */
            ALfloat Gain() const;
            
            /// Set gain.
            /**
             * @param gain The sound's gain.
             */
            void SetGain(ALfloat gain);
            
            /// Get whether sound should loop.
            /**
             * @return Whether the sound should loop
             */
            ALboolean Looping() const;
            
            /// Set whether sound should loop.
            /**
             * @param loop Whether the sound should loop.
             */
            void SetLooping(ALboolean loop);
            
            /// Play sound.
            void Play();
            
            /// Pause sound.
            void Pause();
            
            /// Stop sound.
            void Stop();
            
            /// Get state.
            /**
             * @return One of AL_INITIAL, AL_PLAYING, AL_PAUSED or AL_STOPPED.
             */
            ALint State() const;
            
        private:
            SoundBuffer* mBuffer;
            ALuint mSource;
            
            glm::vec3 mPosition;
            ALfloat mPitch;
            ALfloat mGain;
            ALboolean mLoop;
    };
}
