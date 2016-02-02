#include "Turret.hpp"

Turret::Turret() {

    mTurret = nullptr;
    mMuzzles = nullptr;

}
Turret::Turret(Entity* turret, Entity* muzzle) {

    mTurret = turret;
    mMuzzles = muzzle;

}

Turret::~Turret() {



}

void Turret::SetAngle(float angle) {

    if(mTurret->GetComponent<Component::Transform>() != nullptr)
        mTurret->GetComponent<Component::Transform>()->yaw = angle;

}

void Turret::Shoot() {

    

}