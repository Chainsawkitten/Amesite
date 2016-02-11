#pragma once

#include <Scene/Scene.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/SoundSystem.hpp>

namespace Audio {
    class VorbisFile;
    class SoundBuffer;
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
        // PhysicsSystem.
        System::PhysicsSystem mPhysicsSystem;
        
        System::SoundSystem mSoundSystem;
        
        Audio::SoundBuffer* mTestSoundBuffer;
        
        Entity* s1;
};
