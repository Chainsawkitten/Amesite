#include "ControllerSystem.hpp"

#include <Engine\Scene\Scene.hpp>
#include <Engine\Entity\Entity.hpp>
#include <Engine\Component\Transform.hpp>
#include <Engine\Component\Physics.hpp>

#include "..\Component\Controller.hpp"
#include "..\ControlSchemes.hpp"

using namespace System;

ControllerSystem::ControllerSystem() {



}

ControllerSystem::~ControllerSystem() {



}

void ControllerSystem::Update(Scene& scene, float deltaTime) {

    Component::Controller* a;

    std::vector<Component::Controller*> controllerObjects;
    controllerObjects = scene.GetAll<Component::Controller>();

    for (unsigned int i = 0; i < controllerObjects.size(); i++)
        controllerObjects[i]->ControlScheme(controllerObjects[i], deltaTime);

}
