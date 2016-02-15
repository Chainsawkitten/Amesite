#pragma once
#include <Engine/Scene/Scene.hpp>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
#include <System/ParticleSystem.hpp>
#include "Game/System/HealthSystem.hpp"
#include "Game/System/DamageSystem.hpp"
#include "Game/System/ControllerSystem.hpp"
#include "Game/System/LifeTimeSystem.hpp"

#include <vector>

class PostProcessing;
class FXAAFilter;
class GammaCorrectionFilter;
class Entity;
namespace GameObject {
    class Cave;
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
        
        // Vector containing players
        std::vector<Entity*> mPlayers;
        
        // The main camera
        Entity* mMainCamera;
        
        // The cave
        GameObject::Cave* cave;
        
        // Post processing.
        PostProcessing* postProcessing;
        FXAAFilter* fxaaFilter;
        GammaCorrectionFilter* gammaCorrectionFilter;
        
        // Grid collision
        bool GridCollide(Entity* entity, float deltaTime);
};