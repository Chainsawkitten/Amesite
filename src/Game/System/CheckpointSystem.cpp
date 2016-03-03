#include "CheckpointSystem.hpp"
#include "..\GameObject\Player.hpp"
#include "..\Component\Health.hpp"
#include <Component\Transform.hpp>
#include <Entity\Entity.hpp>
#include <glm\glm.hpp>
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

void System::CheckpointSystem::AddPlayer(GameObject::Player* player) {
    mPlayers.push_back(player);
}

void System::CheckpointSystem::RespawnPlayers() {
    for (auto &player : mPlayers) {
        player->node->GetComponent<Component::Transform>()->position = glm::vec3(mPosition.x, 0.f, mPosition.y);
        player->node->GetComponent<Component::Health>()->health = player->node->GetComponent<Component::Health>()->maxHealth;
        player->Activate();
    }
}
