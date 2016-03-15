#include "SuperEnemy.hpp"

#include <glm/glm.hpp>

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include "../../Util/Hub.hpp"
#include "../Player/Player1.hpp"
#include "../Player/Player2.hpp"
#include "../../Component/Health.hpp"

#include <Util\Log.hpp>

using namespace GameObject;

SuperEnemy::SuperEnemy(Scene* scene) : SuperGameObject(scene) {
    mActive = false;
    node = CreateEntity();
    range = 35.f;
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

    if (!Active() && (isWithinRange || node->GetComponent<Component::Health>()->damaged != -1))
        Activate();
    else if (Active() && !isWithinTwiceRange)
        Deactivate();
}
