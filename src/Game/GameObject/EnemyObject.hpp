#pragma once
#include "GameObject.hpp"

class EnemyObject : public GameObject{
    public:
        void Update();

        ///Enemy's maximum speed
        float maxSpeed;
        
        ///Enemy's acceleration
        float acceleration;

        ///Enemy's hitPoints
        int hitPoints;
};