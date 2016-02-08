#include "Animation.hpp"

//#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

Animation::Animation(Entity* entity) : SuperComponent(entity) {
    activeAnimationClip = nullptr;
}

Animation::~Animation() {
    for (AnimationClip* animationClip : animationClipVector)
        delete animationClip;
    animationClipVector.clear();
    animationClipVector.shrink_to_fit();
}
