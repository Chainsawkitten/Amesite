#include "SuperBoss.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>

using namespace GameObject;

SuperBoss::SuperBoss(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
}

SuperBoss::~SuperBoss() {
}
