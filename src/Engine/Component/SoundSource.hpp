#pragma once

#include "SuperComponent.hpp"
#include <AL/al.h>

class Entity;
namespace Audio {
    class SoundBuffer;
}
namespace System {
    class SoundSystem;
}

namespace Component {
    /// %Component describing a sound source.
    class SoundSource : public SuperComponent {
        friend System::SoundSystem;
        
        public:
            /// Create new sound source.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            SoundSource(Entity* entity);
            
            /// Destructor.
            ~SoundSource();
            
            /// Play the sound.
            void Play();
            
            /// Pause the sound.
            void Pause();
            
            /// Stop the sound.
            void Stop();
            
            /// Sound buffer.
            Audio::SoundBuffer* soundBuffer;
            
            /// Pitch.
            ALfloat pitch;
            
            /// Gain.
            ALfloat gain;
            
            /// Whether the sound should loop.
            ALboolean loop;
            
        private:
            ALuint mSource;
            
            bool mShouldPlay;
            bool mShouldPause;
            bool mShouldStop;
            
            bool mSoundBufferSet;
    };
}
