#pragma once

#include <util\Input.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component\Physics.hpp>

#include "../Turret/Turret.hpp"

class Player {

public:
    Player();
    Player(Entity* playerEntity,float acceleration = 1, InputHandler::Player playerID = InputHandler::PLAYER_ONE);

    ~Player();

    void Update(float dTime);

    void SetTurret(Entity* turret, Entity* muzzle);
    void SetTurret(Turret* turret);

private:
    Entity* mPlayerEntity;
    Turret* mTurret;
    InputHandler::Player mPlayerID;

    float mAcceleration;

};