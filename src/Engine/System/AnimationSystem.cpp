#include "AnimationSystem.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Animation.hpp"
#include "../Component/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace System;

AnimationSystem::AnimationSystem() {
}

AnimationSystem::~AnimationSystem() {
}

glm::mat4 AnimationSystem::GetOrientation(float yaw, float pitch, float roll) {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(yaw), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(pitch), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(roll), glm::vec3(0, 0, 1));
    return orientation;
}

float AnimationSystem::LerpTime(float animationTime, float frameTime) {
    return std::min(animationTime / frameTime, 1.f);
}

float AnimationSystem::SlerpTime(float animationTime, float frameTime) {
    return std::min((1 - glm::cos(3.14f / frameTime * animationTime)) / 2, 1.f);
}

glm::vec3 AnimationSystem::Lerp(glm::vec3 p0, glm::vec3 p1, float t) {
    return p0 * (1 - t) + p1 * t;
}

glm::vec3 AnimationSystem::Slerp(glm::vec3 p0, glm::vec3 p1, float t) {
    float slerpAngle = glm::acos(glm::dot(glm::normalize(p0), glm::normalize(p1)));
    if (glm::sin(slerpAngle) > 0.01f)
        return glm::sin((1 - t) * slerpAngle) / glm::sin(slerpAngle) * p0 + glm::sin(t * slerpAngle) / glm::sin(slerpAngle) * p1;
    return Lerp(p0, p1, t);
}

void AnimationSystem::Update(Scene& scene, float deltaTime) {
    std::list<Component::Animation*> animations = scene.GetAll<Component::Animation>();
    for (auto animationComponent : animations) {
        if (animationComponent->IsKilled())
            continue;
        
        Component::Transform* transformComponent = animationComponent->entity->GetComponent<Component::Transform>();
        if (transformComponent == nullptr)
            return;
        Component::Animation::AnimationClip* animationClip = animationComponent->GetActiveAnimationClip();
        if (animationClip != nullptr && !animationClip->pause) {
            std::vector<Component::Animation::KeyFrame>* keyFrameVector = &animationClip->keyFrameVector;
            if (keyFrameVector->size() < 2)
                animationComponent->animationMatrix = transformComponent->modelMatrix;
            else {
                Component::Animation::KeyFrame* oldKeyFrame = &keyFrameVector->at((animationClip->activeKeyFrame - 1) % keyFrameVector->size());
                Component::Animation::KeyFrame* newKeyFrame = &keyFrameVector->at(animationClip->activeKeyFrame);

                animationClip->animationTime += deltaTime * animationClip->speed;
                float interpolation;
                if (newKeyFrame->slerpTime)
                    interpolation = SlerpTime(animationClip->animationTime, newKeyFrame->time);
                else
                    interpolation = LerpTime(animationClip->animationTime, newKeyFrame->time);
                float invInterpolation = 1.f - interpolation;

                glm::vec3 position;
                if (newKeyFrame->slerp)
                    position = Slerp(oldKeyFrame->position, newKeyFrame->position, interpolation);
                else
                    position = Lerp(oldKeyFrame->position, newKeyFrame->position, interpolation);

                float yaw = newKeyFrame->yaw * interpolation + oldKeyFrame->yaw * invInterpolation;
                float pitch = newKeyFrame->pitch * interpolation + oldKeyFrame->pitch * invInterpolation;
                float roll = newKeyFrame->roll * interpolation + oldKeyFrame->roll * invInterpolation;

                animationComponent->orientationMatrix = GetOrientation(yaw, pitch, roll);
                animationComponent->animationMatrix = glm::translate(glm::mat4(), position) * animationComponent->orientationMatrix;// *glm::scale(glm::mat4(), transformComponent->scale);

                if (interpolation > 0.99f) {
                    animationClip->activeKeyFrame = (animationClip->activeKeyFrame + 1) % keyFrameVector->size();
                    animationClip->animationTime = 0.f;
                }
            }
        }
    }
}
