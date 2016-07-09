#include "WalkerAI.hpp"

#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>

using namespace Component;

WalkerAI::WalkerAI(Entity* entity) : SuperComponent(entity) {
    mThreshold = 1.f;
    mTarget = 0;
    mForward = true;
}

WalkerAI::~WalkerAI(){
    mPoints.clear();
}

void WalkerAI::Update(float deltaTime) {
    if (mPoints.size() != 0) {
        Physics* physics = entity->GetComponent<Physics>();
        
        if (mPoints.size() != 1) {
            if (glm::distance(entity->GetComponent<Transform>()->position, mPoints[mTarget]) < mThreshold) {
                if (mTarget + 1 == mPoints.size())
                    mForward = false;
                else if (mTarget == 0)
                    mForward = true;

                if (mForward)
                    mTarget++;
                else
                    mTarget--;
            }
        }

        physics->acceleration += glm::normalize(mPoints[mTarget] - entity->GetComponent<Transform>()->position) * mSpeed;
    }
}

void WalkerAI::AddPoint(glm::vec3 point) {
    mPoints.push_back(point);
}
