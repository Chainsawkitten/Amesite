#pragma once

#include <AL/alc.h>

namespace Audio {
    class Listener;
    
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
            Listener* GetListener() const;
    
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
    
        private:
            static SoundSystem* mInstance;
    
            ALCdevice *mDevice;
            ALCcontext *mContext;
    
            Listener* mListener;
    };
}
