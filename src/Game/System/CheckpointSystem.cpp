#include "CheckpointSystem.hpp"
#include "../GameObject/Player/SuperPlayer.hpp"
#include <Game/Util/GameEntityFactory.hpp>

#include "../Component/Health.hpp"

#include <Component\ParticleEmitter.hpp>
#include <Component/Transform.hpp>
#include <Entity/Entity.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <System/SoundSystem.hpp>
#include <Audio/SoundBuffer.hpp>
#include <Resources.hpp>
#include "Game/GameObject/Player/SuperPlayer.hpp"

void System::CheckpointSystem::Update(float deltaTime) {
    for (auto& thisPlayer : mPlayers) {
        for (auto& otherPlayer : mPlayers) {
            //If the other player isn't this player and isn't active, and the players are close enough, start healing.
            if (thisPlayer != otherPlayer) {
                if (thisPlayer->Active() && !otherPlayer->Active() && glm::distance(thisPlayer->GetPosition(), otherPlayer->GetPosition()) < 15.f) {
                    otherPlayer->respawnTimeLeft -= deltaTime;
                    otherPlayer->GetNodeEntity()->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.3f, 1.f, 0.3f);
                }
                else {
                    otherPlayer->GetNodeEntity()->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.01f, 0.01f, 0.01f);
                    otherPlayer->respawnTimeLeft = glm::min(otherPlayer->initalRespawnTime, otherPlayer->respawnTimeLeft + deltaTime);
                }
            }
        }
        //If the players respawn timer is < 0, then the player should be activated.
        if (thisPlayer->respawnTimeLeft < 0.001f) {
            thisPlayer->Activate();
        }
    }

    bool anyPlayerHit = false;
    for (auto &player : mPlayers) {
        if (player->mState == 2) {
            anyPlayerHit = true;
            break;
        }
    }

    if (anyPlayerHit && !mPlayingBeepSound) {
        alSourcePlay(mBeepSource);
        mPlayingBeepSound = true;
    }


    for (auto &player : mPlayers) {
        if (player->Active())
            return;
    }

    RespawnPlayers();
}

System::CheckpointSystem::CheckpointSystem() {
    mLowHPSoundBuffer = Resources().CreateSound("Resources/LowHPBeep.ogg");
    alGenSources(1, &mBeepSource);
    alSourcei(mBeepSource, AL_BUFFER, mLowHPSoundBuffer->Buffer());
    alSourcei(mBeepSource, AL_LOOPING, AL_TRUE);
    mPlayingBeepSound = false;
}

System::CheckpointSystem::~CheckpointSystem() {
    alDeleteSources(1, &mBeepSource);
    Resources().FreeSound(mLowHPSoundBuffer);
}

void System::CheckpointSystem::MoveCheckpoint(glm::vec2 position) {
    mPosition = position;
}

void System::CheckpointSystem::AddPlayer(GameObject::SuperPlayer* player) {
    mPlayers.push_back(player);
}

void System::CheckpointSystem::RespawnPlayers() {

    Entity* site1 = GameEntityCreator().CreateCrashSite1();

    site1->GetComponent<Component::Transform>()->position = mPlayers[0]->GetPosition();
    site1->GetComponent<Component::Transform>()->Move(0, -11.f, 0);
    site1->GetComponent<Component::Transform>()->Rotate(rand() % 360, rand() % 360, rand() % 360);

    Entity* site2 = GameEntityCreator().CreateCrashSite2();

    site2->GetComponent<Component::Transform>()->position = mPlayers[1]->GetPosition();
    site2->GetComponent<Component::Transform>()->Move(0, -11.f, 0);
    site2->GetComponent<Component::Transform>()->Rotate(rand() % 360, rand() % 360, rand() % 360);

    for (auto &player : mPlayers) {
        player->SetPosition(glm::vec3(mPosition.x, 0.f, mPosition.y));
        player->GetNodeEntity()->GetComponent<Component::Health>()->health = player->GetNodeEntity()->GetComponent<Component::Health>()->maxHealth;
        player->Activate();
    }
}
