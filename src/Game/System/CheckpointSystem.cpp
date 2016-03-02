#include "CheckpointSystem.hpp"
#include "../GameObject/Player/SuperPlayer.hpp"
#include "../Component/Health.hpp"
#include <Component/Transform.hpp>
#include <Entity/Entity.hpp>
#include <glm/glm.hpp>
#include <vector>

void System::CheckpointSystem::Update() {
    for (auto &player : mPlayers) {
        if (player->GetHealth() > 0.001f)
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
        player->SetPosition(glm::vec3(mPosition.x, 0.f, mPosition.y));
        player->GetNodeEntity()->GetComponent<Component::Health>()->health = player->GetNodeEntity()->GetComponent<Component::Health>()->maxHealth;
        player->Activate();
        //player->GetNodeEntity.GetComponent<Component::Health>()->health = player->GetNodeEntity.GetComponent<Component::Health>()->maxHealth;
    }
}
