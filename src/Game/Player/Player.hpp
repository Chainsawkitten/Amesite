#pragma once

#include <util\Input.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Component/Transform.hpp>

class Player {

public:
    Player();
    Player(Entity* playerEntity,float acceleration = 1, InputHandler::Player playerID = InputHandler::PLAYER_ONE);

    ~Player();

    void Update(float dTime);

private:
    Entity* mPlayerEntity;
    Entity* mTurretEntity;
    InputHandler::Player mPlayerID;

    float mAcceleration;

};