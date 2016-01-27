#include "RenderSystem.hpp"

#include <Engine/Entity/Entity.hpp>

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {

}

void RenderSystem::Render(Entity* entityArr, unsigned int nrOfEntities) {
    for (unsigned int i = 0; i < nrOfEntities; i++) {
        if (entityArr[i].GetComponent<Component::Lens>() != nullptr && entityArr[i].GetComponent<Component::Transform>()) {
            //CAMERA
            Entity* camera = &entityArr[i];
            //TODO Render from camera
        }
    }

}