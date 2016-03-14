#include "CheckpointSystem.hpp"
#include "../GameObject/Player/SuperPlayer.hpp"
#include <Game/Util/GameEntityFactory.hpp>

#include "../Component/Health.hpp"

#include <Component\ParticleEmitter.hpp>
#include <Component/Transform.hpp>
#include <Entity/Entity.hpp>
#include <glm/glm.hpp>
#include <vector>

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

    for (auto &player : mPlayers) {
        if (player->Active())
            return;
    }

    RespawnPlayers();
}

void System::CheckpointSystem::MoveCheckpoint(glm::vec2 position) {
    mPosition = position;
}

void System::CheckpointSystem::AddPlayer(GameObject::SuperPlayer* player) {
    mPlayers.push_back(player);
}

void System::CheckpointSystem::RespawnPlayers() {

    for (auto &player : mPlayers) {
        Entity* site;
        if (typeid(*player).name() == typeid(GameObject::Player1).name())
            site = GameEntityCreator().CreateCrashSite1();
        else if (typeid(*player).name() == typeid(GameObject::Player2).name())
            site = GameEntityCreator().CreateCrashSite2();
        site->GetComponent<Component::Transform>()->position = player->GetPosition();
        site->GetComponent<Component::Transform>()->Move(0, -5.f, 0);
        site->GetComponent<Component::Transform>()->Rotate(rand() % 360, rand() % 360, rand() % 360);

        player->SetPosition(glm::vec3(mPosition.x, 0.f, mPosition.y));
        player->GetNodeEntity()->GetComponent<Component::Health>()->health = player->GetNodeEntity()->GetComponent<Component::Health>()->maxHealth;
        player->Activate();
    }
}
