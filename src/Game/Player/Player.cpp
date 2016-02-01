#include "Player.hpp"

#include <glm/glm.hpp>

Player::Player() {

    mPlayerEntity = nullptr;

}

Player::Player(Entity* playerEntity, float acceleration, InputHandler::Player playerID) {

    mPlayerEntity = playerEntity;
    mAcceleration = acceleration;
    mPlayerID = playerID;

}

Player::~Player() {



}

void Player::Update(float dTime) {
    
    //Move the player
    float x = Input()->ButtonValue(Input()->MOVE_X, mPlayerID);
    float z = Input()->ButtonValue(Input()->MOVE_Z, mPlayerID);

    if (glm::abs(x) + glm::abs(z) > 0.2f)
        mPlayerEntity->GetComponent<Component::Transform>()->Move(glm::vec3(x * mAcceleration * dTime, 0, -z * mAcceleration * dTime));

}