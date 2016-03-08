#include "SuperBoss.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>

#include "../../Util/Hub.hpp"
#include "../Player/Player1.hpp"
#include "../Player/Player2.hpp"

using namespace GameObject;

SuperBoss::SuperBoss(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
}

SuperBoss::~SuperBoss() {
}

bool SuperBoss::Active() {
    return mActive;
}

void SuperBoss::Activate() {
    mActive = true;
}

void SuperBoss::Deactivate() {
    mActive = false;
}

void SuperBoss::mUpdateFunction() {
    GameObject::Player1* player1 = HubInstance().GetPlayer1();
    GameObject::Player2* player2 = HubInstance().GetPlayer2();
    glm::vec3 transformWorldPosition = node->GetComponent<Component::Transform>()->position;
    if (!this->Active() && (player1->Active() || player2->Active())) {
        if (player1->Active() && glm::distance(player1->GetPosition(), transformWorldPosition) < range) {
            this->Activate();
        }
        else if (glm::distance(player2->GetPosition(), transformWorldPosition) < range) {
            this->Activate();
        }
    } else if (this->Active()) {
        if (player1->Active() && glm::distance(player1->GetPosition(), transformWorldPosition) < range * 2) {
            this->Deactivate();
        }
        else if (glm::distance(player2->GetPosition(), transformWorldPosition) < range * 2) {
            this->Deactivate();
        }
    }
}
