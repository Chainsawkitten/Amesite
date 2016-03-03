#include "ExplodeSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include "../Component/Explode.hpp"
#include <Component/Transform.hpp>
#include <Component/SoundSource.hpp>
#include "../GameObject/Explosion.hpp"
#include "../Component/LifeTime.hpp"

#include "../Util/GameEntityFactory.hpp"
#include <Resources.hpp>
#include <Audio/SoundBuffer.hpp>
#include <ctime>

using namespace System;

ExplodeSystem::ExplodeSystem() {
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion01.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion02.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion03.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion04.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion05.ogg"));
    
    mRNG.seed(time(0));
}

ExplodeSystem::~ExplodeSystem() {
    for (Audio::SoundBuffer* sound : mExplosionSounds) {
        Resources().FreeSound(sound);
    }
}

void ExplodeSystem::Update(Scene& scene) {
    std::list<Entity*>* entities = scene.GetList<Entity>();
    for (Entity* entity : *entities) {
        if (entity->IsKilled()) {
            Component::Explode* explodeComp = entity->GetComponent<Component::Explode>();
            if (explodeComp != nullptr) {
                GameEntityCreator().SetScene(&scene);
                GameObject::Explosion* explosion = GameEntityCreator().CreateExplosion(entity->GetComponent<Component::Transform>()->GetWorldPosition() + explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex);
                
                // Create temporary sound.
                if (explodeComp->sound) {
                    Entity* explosionSound = scene.CreateEntity();
                    Component::Transform* transform = explosionSound->AddComponent<Component::Transform>();
                    transform->position = explosion->node->GetComponent<Component::Transform>()->position;
                    Component::SoundSource* soundSource = explosionSound->AddComponent<Component::SoundSource>();
                    
                    // Use random explosion sound.
                    std::uniform_int_distribution<uint32_t> distribution(0, mExplosionSounds.size()-1);
                    soundSource->soundBuffer = mExplosionSounds[distribution(mRNG)];
                    soundSource->gain = 15.f;
                    soundSource->Play();
                    
                    Component::LifeTime* lifetime = explosionSound->AddComponent<Component::LifeTime>();
                    lifetime->lifeTime = 6.f;
                }
            }
        }
    }
}
