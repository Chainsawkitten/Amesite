#include "Controller.hpp"

#include <Entity\Entity.hpp>
#include <Component\Physics.hpp>

using namespace Component;

Controller::Controller(Entity* entity) : SuperComponent(entity) {
    playerID = InputHandler::PLAYER_ONE;
    device = InputHandler::NONE;
    speed = 100.f;
    enabled = true;
}

Controller::~Controller() {
    
}
