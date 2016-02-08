#pragma once
#include <Engine/Scene/Scene.hpp>

#include <System/RenderSystem.hpp>
#include <System/PhysicsSystem.hpp>
#include <System/CollisionSystem.hpp>
//#include <System/ParticleSystem.hpp>
//#include <System/ParticleRenderSystem.hpp>
#include "Game/System/HealthSystem.hpp"
#include "Game/System/DamageSystem.hpp"
#include "Game/System/ControllerSystem.hpp"

#include <vector>

class Entity;

class MainScene : public Scene {
    public:
        void Update(float deltaTime);
        void Init();
        MainScene();
        ~MainScene();

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

        // ParticleSystem
        System::ParticleSystem* mParticleSystem;

        // Vector containing players
        std::vector<Entity*> mPlayers;

        // The main camera
        Entity* mMainCamera;
};
