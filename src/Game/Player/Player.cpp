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

    glm::vec3 speedVec = glm::vec3(x * mAcceleration * dTime, 0, z * mAcceleration * dTime);

    //If there's a physics component attached we use it to move.
    if (mPlayerEntity->GetComponent<Component::Physics>() != NULL) {

        if (glm::abs(x) + glm::abs(z) > 0.3f)
            mPlayerEntity->GetComponent<Component::Physics>()->acceleration = speedVec;
        else
            mPlayerEntity->GetComponent<Component::Physics>()->acceleration = glm::vec3(0, 0, 0);
    
    }
    else if (glm::abs(x) + glm::abs(z) > 0.3f)
        mPlayerEntity->GetComponent<Component::Transform>()->Move(glm::vec3(x * mAcceleration * dTime, 0, z * mAcceleration * dTime));

    //Rotate the turret(s)
    float a = Input()->ButtonValue(Input()->AIM_Z, mPlayerID);
    float b = Input()->ButtonValue(Input()->AIM_X, mPlayerID);
    float rot = 0;

    if (glm::abs(a) + glm::abs(b) > 0.2f) {

        if (a >= 0)
            rot = -glm::atan(b / a) * 360.f / (2 * 3.14);
        else
            rot = 180 - glm::atan(b / a) * 360.f / (2 * 3.14);

        if (mTurret != nullptr)
            mTurret->SetAngle(rot);

    }

}

void Player::SetTurret(Entity* turret, Entity* muzzle) {

    mTurret = new Turret(turret, muzzle);

}
void Player::SetTurret(Turret* turret) {

    mTurret = turret;

}
