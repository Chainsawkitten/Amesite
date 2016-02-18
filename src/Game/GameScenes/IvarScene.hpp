//#pragma once
//#include <Engine/Scene/Scene.hpp>
//
//#include <System/RenderSystem.hpp>
//#include <System/PhysicsSystem.hpp>
//#include <System/CollisionSystem.hpp>
//#include <System/ParticleSystem.hpp>
//#include <System/AnimationSystem.hpp>
//#include <System/SoundSystem.hpp>
//#include "Game/System/HealthSystem.hpp"
//#include "Game/System/DamageSystem.hpp"
//#include "Game/System/ControllerSystem.hpp"
//#include "Game/System/LifeTimeSystem.hpp"
//
//
//#include <vector>
//
//class PostProcessing;
//class FXAAFilter;
//class GammaCorrectionFilter;
//class Entity;
//
//namespace GameObject {
//    class Cave;
//    class Player;
//    class Camera;
//}
//
///// The main scene for the game.
//class IvarScene : public Scene {
//    public:
//        ///Constructor
//        IvarScene();
//        
//        ///Destructor
//        ~IvarScene();
//        
//        /// Update the scene.
//        /**
//         *@param deltaTime Time since last frame.
//         */
//        void Update(float deltaTime);
//        
//    private:
//        // SoundSystem.
//        System::SoundSystem mSoundSystem;
//
//        // PhysicsSystem.
//        System::PhysicsSystem mPhysicsSystem;
//        
//        // ControllerSystem.
//        System::ControllerSystem mControllerSystem;
//        
//        // HealthSystem.
//        System::HealthSystem mHealthSystem;
//        
//        // DamageSystem.
//        System::DamageSystem mDamageSystem;
//        
//        // RenderSystem.
//        System::RenderSystem mRenderSystem;
//        
//        // CollisionSystem.
//        System::CollisionSystem mCollisionSystem;
//        
//        // The life time system
//        System::LifeTimeSystem mLifeTimeSystem;
//
//        // The life time system
//        System::AnimationSystem mAnimationSystem;
//        
//        // Vector containing players
//        std::vector<GameObject::Player*> mPlayers;
//        
//        // The main camera
//        GameObject::Camera* mMainCamera;
//        
//        // The cave
//        GameObject::Cave* mCave;
//        
//        // Post processing.
//        PostProcessing* postProcessing;
//        FXAAFilter* fxaaFilter;
//        GammaCorrectionFilter* gammaCorrectionFilter;
//        
//        // Grid collision
//        bool GridCollide(Entity* entity, float deltaTime);
//};
