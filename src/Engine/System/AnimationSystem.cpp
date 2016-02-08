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

glm::mat4 GetOrientation(float yaw, float pitch, float roll) {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(yaw), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(pitch), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(roll), glm::vec3(0, 0, 1));
    return orientation;
}

void AnimationSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Animation*> animationVector = scene.GetAll<Component::Animation>();
    for (auto animationComponent : animationVector) {
        Component::Transform* transformComponent = animationComponent->entity->GetComponent<Component::Transform>();
        // Check nullptr?
        Component::Animation::AnimationClip* animationClip = animationComponent->activeAnimationClip;
        std::vector<Component::Animation::KeyFrame>* keyFrameVector = &animationClip->keyFrameVector;
        if (keyFrameVector->size() < 2)
            animationComponent->animationMatrix = transformComponent->modelMatrix;
        else {
            Component::Animation::KeyFrame* oldKeyFrame = &keyFrameVector->at((animationClip->activeKeyFrame - 1) % keyFrameVector->size());
            Component::Animation::KeyFrame* newKeyFrame = &keyFrameVector->at(animationClip->activeKeyFrame);
            
            animationClip->animationTime += deltaTime;
            float interpolation = std::min(animationClip->animationTime / newKeyFrame->time, 1.f);
            float invInterpolation = 1 - interpolation;

            glm::vec3 position = newKeyFrame->position * interpolation + oldKeyFrame->position * invInterpolation;
            float yaw = newKeyFrame->yaw * interpolation + oldKeyFrame->yaw * invInterpolation;
            float pitch = newKeyFrame->pitch * interpolation + oldKeyFrame->pitch * invInterpolation;
            float roll = newKeyFrame->roll * interpolation + oldKeyFrame->roll * invInterpolation;

            animationComponent->animationMatrix = glm::translate(glm::mat4(), position) * GetOrientation(yaw, pitch, roll) * glm::scale(glm::mat4(), transformComponent->scale);

            if (interpolation > 0.99f) {
                animationClip->activeKeyFrame = (animationClip->activeKeyFrame + 1) % keyFrameVector->size();
                animationClip->animationTime = 0;
            }
        }
    }
}
