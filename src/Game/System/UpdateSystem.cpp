#include "UpdateSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Update.hpp"

using namespace System;

UpdateSystem::UpdateSystem() {
}

UpdateSystem::~UpdateSystem() {
}

void UpdateSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Update*> UpdateVector = scene.GetAll<Component::Update>();
    for (auto& updateComponent : UpdateVector)
        updateComponent->updateFunction();
}
