#include "LookerAI.hpp"

#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>

#include <util/Log.hpp>

using namespace Component;

LookerAI::LookerAI(Entity* entity) : SuperComponent(entity) {
    mTarget = -1;
}

LookerAI::~LookerAI(){
    mTargets.clear();
}

void LookerAI::Update(float deltaTime) {

    Component::Physics* physics = this->entity->GetComponent<Component::Physics>();

    if (mTargets.size() != 0) {

        if (mTarget == -1) {

            for (int i = 0; i < mTargets.size(); i++) {

                if (glm::distance(this->entity->GetComponent<Component::Transform>()->GetWorldPosition(), mTargets[i]->GetComponent<Component::Transform>()->GetWorldPosition()) <= mDistance) {

                    mTarget = i;
                    break;

                }

            }

        } else {

            Component::Transform* transform = this->entity->GetComponent<Component::Transform>();
            Component::Transform* targetTransform = mTargets[mTarget]->GetComponent<Component::Transform>();

            if (glm::distance(transform->GetWorldPosition(), targetTransform->GetWorldPosition()) < mDistance) {

                float oldAngle = glm::radians(transform->yaw);

                glm::vec3 oldPoint = transform->GetWorldPosition() -+ glm::normalize(glm::vec3(glm::sin(oldAngle), 0, glm::cos(oldAngle))) * 5.f;
                glm::vec3 newPoint = oldPoint + glm::normalize(targetTransform->GetWorldPosition() - oldPoint);
                glm::vec3 oldDirection = glm::normalize(oldPoint - transform->GetWorldPosition());
                glm::vec3 newDirection = glm::normalize(newPoint - transform->GetWorldPosition());
                float dot = glm::dot(glm::vec2(oldDirection.x, oldDirection.z), glm::vec2(newDirection.x, newDirection.z));

                if (dot > 1.f)
                    dot = 1.f;
                else if (dot < -1.f)
                    dot = -1.f;

                float angle = acos(dot);
                if (glm::cross(oldDirection, newDirection).y > 0)
                    angle *= -1;
                    
                Log() << angle << "\n";

                transform->yaw = glm::degrees(oldAngle + angle);

            }

        }

    }
}

void LookerAI::AddEntity(Entity* entity) {

    mTargets.push_back(entity);

}
