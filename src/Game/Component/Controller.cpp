#include "Controller.hpp"

#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Physics.hpp>

using namespace Component;

Controller::Controller(Entity* entity) : SuperComponent(entity) {
    playerID = InputHandler::PLAYER_ONE;
    device = InputHandler::NONE;
    speed = 100.f;
    enabled = true;
}

Controller::~Controller() {
    
}
