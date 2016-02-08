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

class Entity;
namespace GameObject {
    class Cave;
}

class MainScene : public Scene {
    public:
        void Update(float deltaTime);
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
        System::ParticleSystem mParticleSystem;

        // The life time system
        System::LifeTimeSystem mLifeTimeSystem;

        // Vector containing players
        std::vector<Entity*> mPlayers;

        // The main camera
        Entity* mMainCamera;

        // The cave
        GameObject::Cave* cave;

        bool MainScene::GridCollide(Entity* entity, float deltaTime);
};
