#include "WalkerAI.hpp"

#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>

using namespace Component;

WalkerAI::WalkerAI(Entity* entity) : SuperComponent(entity) {

    mThreshold = 1.f;
    mTarget = -1;
    mForward = true;
}

WalkerAI::~WalkerAI(){
    mPoints.clear();
}

void WalkerAI::Update(float deltaTime) {

    Component::Physics* physics = this->entity->GetComponent<Component::Physics>();

    if (mPoints.size() != 0) {

        if (mPoints.size() != 1) {

            if (glm::distance(this->entity->GetComponent<Component::Transform>()->position, mPoints[mTarget]) < mThreshold) {

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

        physics->acceleration += glm::normalize(mPoints[mTarget] - this->entity->GetComponent<Component::Transform>()->position) * mSpeed;

    }

}

void WalkerAI::AddPoint(glm::vec3 point) {

    if (mTarget == -1)
        mTarget = 0;

    mPoints.push_back(point);

}
