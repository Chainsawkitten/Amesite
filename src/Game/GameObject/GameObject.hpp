#pragma once
#include <map>

class Entity;

///An abstract class describing an object within the game
class GameObject {
    public:
        ///A list of entities associated with this object.
        std::map<std::string, Entity*> entityList;

        ///An update method.
        virtual void Update() = 0;
};