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
    GameObject::Player1* player1 = HubInstance().GetPlayer1();
    GameObject::Player2* player2 = HubInstance().GetPlayer2();
    glm::vec3 transformWorldPosition = node->GetComponent<Component::Transform>()->position;
    if (!this->Active() && (player1->Active() || player2->Active())) {
        if (player1->Active() && glm::distance(player1->GetPosition(), transformWorldPosition) < range) {
            this->Activate();
        } else if (glm::distance(player2->GetPosition(), transformWorldPosition) < range) {
            this->Activate();
        }
    }
}
