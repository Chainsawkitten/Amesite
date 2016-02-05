#include "Controller.hpp"

using namespace Component;

#include <Entity\Entity.hpp>
#include <Component\Physics.hpp>

void EmptyControls(Controller* controller, float deltaTime);

Controller::Controller(Entity* entity) : SuperComponent(entity) {

    playerID = InputHandler::PLAYER_ONE;
    ControlScheme = &EmptyControls;
    mSpeed = 1;

}

Controller::~Controller() {



}

void EmptyControls(Controller* controller, float deltaTime) {}