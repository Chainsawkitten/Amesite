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

    range = 80.f;
    mActive = false;
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
    glm::vec3 transformWorldPosition = node->GetComponent<Component::Transform>()->position;
    bool isWithinRange = false;
    bool isWithinTwiceRange = false;
    for (auto& player : HubInstance().mPlayers) {
        if (player->Active()) {
            float distance = glm::distance(player->GetPosition(), transformWorldPosition);
            if (distance < range)
                isWithinRange = true;
            if (distance < range*2.f)
                isWithinTwiceRange = true;
        }
    }

    if (!Active() && isWithinRange)
        Activate();
    else if (Active() && !isWithinTwiceRange)
        Deactivate();
}
