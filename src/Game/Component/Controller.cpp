#include "Controller.hpp"

using namespace Component;

Controller::Controller(Entity* entity) : SuperComponent(entity) {

    playerID = InputHandler::PLAYER_ONE;

}

Controller::~Controller() {



}
