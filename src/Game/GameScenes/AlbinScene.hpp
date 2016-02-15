#pragma once

#include <Scene/Scene.hpp>
#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/SoundSystem.hpp>
#include "../System/ControllerSystem.hpp"

namespace Audio {
    class VorbisFile;
    class SoundBuffer;
}
class PostProcessing;
class FXAAFilter;
class GammaCorrectionFilter;

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
        // RenderSystem.
        System::RenderSystem mRenderSystem;
        
        // PhysicsSystem.
        System::PhysicsSystem mPhysicsSystem;
        
        // ControllerSystem.
        System::ControllerSystem mControllerSystem;
        
        // SoundSystem.
        System::SoundSystem mSoundSystem;
        
        Audio::SoundBuffer* mTestSoundBuffer;
        
        Entity* s1;
        
        // Post processing.
        PostProcessing* postProcessing;
        FXAAFilter* fxaaFilter;
        GammaCorrectionFilter* gammaCorrectionFilter;
        
        // The main camera
        Entity* mMainCamera;
        
        // Vector containing players
        std::vector<Entity*> mPlayers;
};
