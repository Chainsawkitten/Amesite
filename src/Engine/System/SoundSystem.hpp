#pragma once

#include <AL/alc.h>

class Scene;
namespace Audio {
    class Listener;
}
namespace Component {
    class SoundSource;
}

namespace System {
    /// Handles OpenAL sound.
    /**
     * There should only be one instance.
     */
    class SoundSystem {
        public:
            /// Initializes OpenAL sound.
            /**
             * Creates default audio device, audio context and Listener.
             */
            SoundSystem();
            
            /// Destructor.
            /**
             * Closes the audio device and destroys the audio context.
             */
            ~SoundSystem();
            
            /// Get listener.
            /**
             * @return The listener.
             */
            Audio::Listener* GetListener() const;
            
            /// Set main volume.
            /**
             * @param volume New volume.
             */
            void SetVolume(float volume);
            
            /// Get main volume.
            /**
             * @return The main volume.
             */
            float GetVolume() const;
            
            /// Get instance of SoundSystem.
            /**
             * @return The SoundSystem instance.
             */
            static SoundSystem* GetInstance();
            
            /// Check for OpenAL errors.
            /**
             * @param message Message to print to standard error if an error was encountered.
             */
            static void CheckError(const char* message);
            
            /// Update Scene containing entities. Moves sound sources and plays sounds.
            /**
             * @param scene The scene to update.
             */
            void Update(Scene& scene);
            
        private:
            static SoundSystem* mInstance;
            
            ALCdevice *mDevice;
            ALCcontext *mContext;
            
            Audio::Listener* mListener;
            float mVolume;
    };
}
