#include "SuperEnemy.hpp"

#include <glm/glm.hpp>

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../../Util/Hub.hpp"
#include "../Player/Player1.hpp"
#include "../Player/Player2.hpp"

using namespace GameObject;

SuperEnemy::SuperEnemy(Scene* scene) : SuperGameObject(scene) {
    mActive = false;
    node = CreateEntity();
    range = 30.f;
}

SuperEnemy::~SuperEnemy() {
}

bool SuperEnemy::Active() {
    return mActive;
}

void SuperEnemy::Activate() {
    mActive = true;
}


void SuperEnemy::Deactivate() {
    mActive = false;
}

void SuperEnemy::mUpdateFunction() {
    bool anyPlayerActive = false;
    for (auto& player : HubInstance().mPlayers) {
        anyPlayerActive = player->Active();
        if (anyPlayerActive)
            break;
    }



    glm::vec3 transformWorldPosition = node->GetComponent<Component::Transform>()->position;
    if (!this->Active() && (anyPlayerActive) {

        for (auto& player : HubInstance().mPlayers) {
            if(player)
        }
        //if (player1->Active() && glm::distance(player1->GetPosition(), transformWorldPosition) < range) {
        //    this->Activate();
        //} else if (glm::distance(player2->GetPosition(), transformWorldPosition) < range) {
        //    this->Activate();
        //}
    }
}
