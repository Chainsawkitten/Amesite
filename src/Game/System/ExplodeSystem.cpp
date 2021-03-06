#include "ExplodeSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Explode.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/SoundSource.hpp>
#include "../GameObject/Explosion.hpp"
#include "../Component/LifeTime.hpp"

#include "../Util/GameEntityFactory.hpp"
#include <Engine/Resources.hpp>
#include <Engine/Audio/SoundBuffer.hpp>
#include <ctime>

using namespace System;

ExplodeSystem::ExplodeSystem() {
    mExplosionSounds.push_back(Resources().CreateSound("Resources/sound/Explosion01.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/sound/Explosion02.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/sound/Explosion03.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/sound/Explosion04.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/sound/Explosion05.ogg"));
    
    mRNG.seed(time(0));
}

ExplodeSystem::~ExplodeSystem() {
    for (Audio::SoundBuffer* sound : mExplosionSounds) {
        Resources().FreeSound(sound);
    }
}

void ExplodeSystem::Update(Scene& scene) {
    std::vector<Entity*>* entities = scene.GetVector<Entity>();
    for (std::size_t i = 0; i < entities->size(); ++i) {
        Entity* entity = (*entities)[i];
        if (entity->IsKilled()) {
            Component::Explode* explodeComp = entity->GetComponent<Component::Explode>();
            if (explodeComp != nullptr) {
                GameEntityCreator().SetScene(&scene);
                GameObject::Explosion* explosion = GameEntityCreator().CreateExplosion(entity->GetComponent<Component::Transform>()->GetWorldPosition() + explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex, explodeComp);
                
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
