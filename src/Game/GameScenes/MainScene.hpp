#pragma once
#include <Scene/Scene.hpp>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
#include <System/AnimationSystem.hpp>
#include "../System/CheckpointSystem.hpp"
#include "../System/HealthSystem.hpp"
#include "../System/DamageSystem.hpp"
#include "../System/ControllerSystem.hpp"
#include "../System/LifeTimeSystem.hpp"
#include "../System/ReflectSystem.hpp"
#include "../System/ExplodeSystem.hpp"
#include "../System/GridCollideSystem.hpp"
#include "../System/EnemySpawnerSystem.hpp"
#include "../System/UpdateSystem.hpp"
#include "../System/SpawnerSystem.hpp"
#include "../Menu/Menu.hpp"
#include <Water/Water.hpp>

#include <AL/al.h>

#include <vector>

class PostProcessing;
class FXAAFilter;
class GammaCorrectionFilter;
class GlowFilter;
class GlowBlurFilter;
class Entity;
namespace GameObject {
    class Cave;
    class Camera;
    class SuperPlayer;
    class SuperBoss;
    class Altar;
    class Pillar;
    class Rock;
}
namespace Audio {
    class SoundBuffer;
}

/// The main scene for the game.
class MainScene : public Scene {
    public:
        ///Constructor
        MainScene();
        
        ///Destructor
        ~MainScene();
        
        /// Update the scene.
        /**
         *@param deltaTime Time since last frame.
         */
        void Update(float deltaTime);
        
    private:
        // PhysicsSystem.
        System::PhysicsSystem mPhysicsSystem;

        // ControllerSystem.
        System::ControllerSystem mControllerSystem;

        // HealthSystem.
        System::HealthSystem mHealthSystem;

        // DamageSystem.
        System::DamageSystem mDamageSystem;

        // RenderSystem.
        System::RenderSystem mRenderSystem;

        // CollisionSystem.
        System::CollisionSystem mCollisionSystem;

        // The life time system
        System::LifeTimeSystem mLifeTimeSystem;

        // The spawner system
        System::SpawnerSystem mSpawnerSystem;

        // The reflect system
        System::ReflectSystem mReflectSystem;

        // The explode system
        System::ExplodeSystem mExplodeSystem;

        // The animation system
        System::AnimationSystem mAnimationSystem;

        // checkpoint system
        System::CheckpointSystem mCheckpointSystem;

        // The grid collide system
        System::GridCollideSystem mGridCollideSystem;

        // The enemy spawner system
        System::EnemySpawnerSystem mEnemySpawnerSystem;
        
        // The update system
        System::UpdateSystem mUpdateSystem;

        // Vector containing players
        std::vector<GameObject::SuperPlayer*> mPlayers;

        // Bosses
        std::vector<GameObject::SuperBoss*> mBossVector;

        // Pillars
        std::vector<GameObject::Pillar*> mPillarVector;

        // The main camera
        GameObject::Camera* mMainCamera;

        // The cave
        GameObject::Cave* mCave;
        
        // Water.
        Water mWater;

        // Post processing.
        PostProcessing* mPostProcessing;
        FXAAFilter* mFxaaFilter;
        GammaCorrectionFilter* mGammaCorrectionFilter;
        GlowFilter* mGlowFilter;
        GlowBlurFilter* mGlowBlurFilter;
        int mBossCounter;
        float mTimer;
        glm::vec2 mPortalPosition;
        
        // Menu
        Menu mMenu;

        // Used to exclude enemy spawning from certain areas.
        std::vector<glm::vec3> mNoSpawnRooms;

        // Music.
        Audio::SoundBuffer* mCalmSoundBuffer;
        ALuint mCalmSource;
        Audio::SoundBuffer* mActionSoundBuffer;
        ALuint mActionSource;
        
        ALfloat mMix;
        ALfloat mTargetMix;
};
