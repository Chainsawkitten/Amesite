#pragma once
#include <Engine/Scene/Scene.hpp>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
#include <System/AnimationSystem.hpp>
#include "Game/System/CheckpointSystem.hpp"
#include "Game/System/HealthSystem.hpp"
#include "Game/System/DamageSystem.hpp"
#include "Game/System/ControllerSystem.hpp"
#include "Game/System/LifeTimeSystem.hpp"
#include "Game/System/ReflectSystem.hpp"
#include "Game/System/ExplodeSystem.hpp"
#include "Game/System/GridCollideSystem.hpp"
#include "Game/System/EnemySpawnerSystem.hpp"
#include "Game/System/UpdateSystem.hpp"
#include "Game/System/SpawnerSystem.hpp"

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

        // Post processing.
        PostProcessing* mPostProcessing;
        FXAAFilter* mFxaaFilter;
        GammaCorrectionFilter* mGammaCorrectionFilter;
        GlowFilter* mGlowFilter;
        GlowBlurFilter* mGlowBlurFilter;
        int mBossCounter;
        float mTimer;
        glm::vec2 mPortalPosition;

        // Used to exclude enemy spawning from certain areas.
        std::vector<glm::vec3> mNoSpawnRooms;

        // Music.
        Audio::SoundBuffer* mMusicSoundBuffer;
        ALuint mSource;
};
