#include "ExplodeSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include "../Component/Explode.hpp"
#include <Component/Transform.hpp>
#include <Component/SoundSource.hpp>
#include "../GameObject/Explosion.hpp"

#include "../Util/GameEntityFactory.hpp"
#include <Resources.hpp>
#include <Audio/SoundBuffer.hpp>

using namespace System;

ExplodeSystem::ExplodeSystem() {
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion01.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion02.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion03.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion04.ogg"));
    mExplosionSounds.push_back(Resources().CreateSound("Resources/Explosion05.ogg"));
}

ExplodeSystem::~ExplodeSystem() {
    for (Audio::SoundBuffer* sound : mExplosionSounds) {
        Resources().FreeSound(sound);
    }
}

void ExplodeSystem::Update(Scene& scene) {
    std::vector<Entity*> killedEntitiesVector = scene.GetKilledEntitesVector();
    for (auto entity : killedEntitiesVector) {
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
                soundSource->soundBuffer = mExplosionSounds[0];
                soundSource->gain = 15.f;
                soundSource->Play();
            }
        }
    }
}
