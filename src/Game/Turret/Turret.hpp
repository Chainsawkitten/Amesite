#pragma once

#include <util\Input.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Component/Transform.hpp>

class Entity;

class Turret {

public:
    Turret();
    Turret(Entity* turret, Entity* muzzle);

    ~Turret();

    void SetAngle(float angle);
    void Shoot();

private:
    Entity* mTurret;
    Entity* mMuzzles;

};