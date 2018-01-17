#include "CheckpointSystem.hpp"

#include "../GameObject/Player/SuperPlayer.hpp"
#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"
#include "../Util/GameEntityFactory.hpp"

#include "../Component/Health.hpp"

#include <Component/ParticleEmitter.hpp>
#include <Component/Transform.hpp>
#include <Entity/Entity.hpp>

#include <glm/glm.hpp>
#include <vector>
#include <System/SoundSystem.hpp>
#include <Audio/SoundBuffer.hpp>
#include <Resources.hpp>
#include "../GameObject/Player/SuperPlayer.hpp"

#include "../Util/Hub.hpp"

System::CheckpointSystem::CheckpointSystem() {
    mLowHPSoundBuffer = Resources().CreateSound("Resources/sound/LowHPBeep.ogg");
    alGenSources(1, &mBeepSource);
    alSourcei(mBeepSource, AL_BUFFER, mLowHPSoundBuffer->Buffer());
    alSourcei(mBeepSource, AL_LOOPING, AL_TRUE);
    mPlayingBeepSound = false;
    mRespawn = false;
    timesDied = 0;
}

void System::CheckpointSystem::Update(float deltaTime) {
    bool anyPlayerHit = false;
    for (auto& thisPlayer : HubInstance().mPlayers) {
        for (auto& otherPlayer : HubInstance().mPlayers) {
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
            thisPlayer->respawnTimeLeft = thisPlayer->initalRespawnTime;
        }

        if (thisPlayer->mState >= 1 && thisPlayer->mState < 3) {
            anyPlayerHit = true;
            break;
        }
    }

    //Turn on/off the hurt sound.
    if (anyPlayerHit && !mPlayingBeepSound) {
        alSourcePlay(mBeepSource);
        mPlayingBeepSound = true;
    } else if (!anyPlayerHit && mPlayingBeepSound) {
        alSourceStop(mBeepSource);
        mPlayingBeepSound = false;
    }


    for (auto &player : HubInstance().mPlayers) {
        if (player->Active())
            return;
    }

    RespawnPlayers();
}

System::CheckpointSystem::~CheckpointSystem() {
    alDeleteSources(1, &mBeepSource);
    Resources().FreeSound(mLowHPSoundBuffer);
}

void System::CheckpointSystem::MoveCheckpoint(glm::vec2 position) {
    mPosition = position;
}

void System::CheckpointSystem::RespawnPlayers() {

    mRespawn = true;

    for (auto &player : HubInstance().mPlayers) {
        Entity* site = nullptr;
        if (typeid(*player).name() == typeid(GameObject::Player1).name())
            site = GameEntityCreator().CreateCrashSite1();
        else if (typeid(*player).name() == typeid(GameObject::Player2).name())
            site = GameEntityCreator().CreateCrashSite2();
        Component::Transform* siteTransform = site->GetComponent<Component::Transform>();
        siteTransform->position = player->GetPosition();
        siteTransform->Move(0, -5.f, 0);
        siteTransform->Rotate(rand() % 360, rand() % 360, rand() % 360);

        player->SetPosition(glm::vec3(mPosition.x, 0.f, mPosition.y));
        player->GetNodeEntity()->GetComponent<Component::Health>()->health = player->GetNodeEntity()->GetComponent<Component::Health>()->maxHealth;
        player->Activate();
    }
    timesDied++;
}
