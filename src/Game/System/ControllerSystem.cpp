#include "ControllerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Controller.hpp"
#include "../Util/ControlSchemes.hpp"

using namespace System;

void ControllerSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Controller*> controllerObjects;
    controllerObjects = scene.GetAll<Component::Controller>();
    
    for (unsigned int i = 0; i < controllerObjects.size(); i++) {
        for (unsigned int scheme = 0; scheme < controllerObjects[i]->controlSchemes.size(); scheme++)
            if(controllerObjects[i]->enabled)
                controllerObjects[i]->controlSchemes[scheme](controllerObjects[i], deltaTime);
    }

}
