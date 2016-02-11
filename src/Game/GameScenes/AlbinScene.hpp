#pragma once

#include <Scene/Scene.hpp>
#include <System/SoundSystem.hpp>

namespace Audio {
    class VorbisFile;
    class SoundBuffer;
    class Sound;
}

/// Albin's testing scene.
class AlbinScene : public Scene {
    public:
        /// Create new testing scene.
        AlbinScene();
        
        /// Destructor.
        ~AlbinScene();
        
        /// Update the scene.
        /**
         * @param deltaTime Time since last frame.
         */
        void Update(float deltaTime);
        
    private:
        System::SoundSystem mSoundSystem;
        
        Audio::SoundBuffer* mTestSoundBuffer;
        Audio::Sound* mTestSoundSource1;
        Audio::Sound* mTestSoundSource2;
};
