#pragma once

#include "SuperComponent.hpp"

class Entity;

#include <glm/glm.hpp>
#include <vector>

namespace Component {
    /// %Component providing animation.
    class Animation : public SuperComponent {
        public:
            /// Create new %Animation.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Animation(Entity* entity);

            /// Destructor.
            ~Animation();

            struct KeyFrame {
                glm::vec3 position;
                float yaw, pitch, roll;
                float time;
            };

            struct AnimationClip {
                std::vector<KeyFrame> keyFrameVector;
                unsigned int activeKeyFrame = 0;
                float animationTime = 0.f;

                void CreateKeyFrame(glm::vec3 position, float yaw, float pitch, float roll, float time) {
                    KeyFrame keyFrame;
                    keyFrame.position = position;
                    keyFrame.yaw = yaw;
                    keyFrame.pitch = pitch;
                    keyFrame.roll = roll;
                    keyFrame.time = time;
                    keyFrameVector.push_back(keyFrame);
                }
            };

            AnimationClip* CreateAnimationClip() {
                AnimationClip* animationClip = new AnimationClip();
                animationClipVector.push_back(animationClip);
                if (animationClipVector.size() == 1)
                    activeAnimationClip = animationClip;
                return animationClip;
            }

            std::vector<AnimationClip*> animationClipVector;

            /// Active animation;
            /**
             * Default: nullptr
             */
            AnimationClip* activeAnimationClip;

            /// Animation matrix, like model matrix but manipulated with animation.
            /**
             * Default: Identity matrix
             */
            glm::mat4 animationMatrix;
        };
}
