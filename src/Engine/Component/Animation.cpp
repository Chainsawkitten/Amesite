#include "Animation.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

Animation::Animation(Entity* entity) : SuperComponent(entity) {
    mActiveAnimationClip = nullptr;
}

Animation::~Animation() {
    for (auto& it : mAnimationClipMap)
        delete it.second;
}

void Animation::Start(const std::string key) {
    std::map<std::string, Animation::AnimationClip*>::iterator it = mAnimationClipMap.find(key);
    if (it != mAnimationClipMap.end()) {
        if (mActiveAnimationClip != nullptr) {
            mActiveAnimationClip->activeKeyFrame = 0;
            mActiveAnimationClip->animationTime = 0.f;
            mActiveAnimationClip->pause = false;
        }
        mActiveAnimationClip = it->second;
    }
}

void Animation::Pause() {
    if (mActiveAnimationClip != nullptr)
        mActiveAnimationClip->pause = true;
}

void Animation::Play() {
    if (mActiveAnimationClip != nullptr)
        mActiveAnimationClip->pause = false;
}

void Animation::Stop() {
    if (mActiveAnimationClip != nullptr) {
        mActiveAnimationClip->activeKeyFrame = 0;
        mActiveAnimationClip->animationTime = 0.f;
        mActiveAnimationClip->pause = false;
    }
}

Animation::AnimationClip* Animation::CreateAnimationClip(const std::string key) {
    std::map<std::string, Animation::AnimationClip*>::iterator it = mAnimationClipMap.find(key);
    if (it != mAnimationClipMap.end())
        return it->second;
    AnimationClip* animationClip = new AnimationClip();
    mAnimationClipMap[key] = animationClip;
    return animationClip;
}

void Animation::AnimationClip::CreateKeyFrame(glm::vec3 position, float yaw, float pitch, float roll, float time, bool slerp, bool slerpTime) {
    KeyFrame keyFrame;
    keyFrame.position = position;
    keyFrame.yaw = yaw;
    keyFrame.pitch = pitch;
    keyFrame.roll = roll;
    keyFrame.time = time;
    keyFrame.slerp = slerp;
    keyFrame.slerpTime = slerpTime;
    keyFrameVector.push_back(keyFrame);
}

Animation::AnimationClip* Animation::GetActiveAnimationClip() {
    return mActiveAnimationClip;
}
