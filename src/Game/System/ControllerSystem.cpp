#include "ControllerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Controller.hpp"
#include "../Util/ControlSchemes.hpp"

using namespace System;

void ControllerSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Controller*> controllerObjects = scene.GetAll<Component::Controller>();
    for (Component::Controller* controller : controllerObjects) {
        for (unsigned int scheme = 0; scheme < controller->controlSchemes.size(); scheme++)
            if (controller->enabled)
                controller->controlSchemes[scheme](controller, deltaTime);
    }

}
