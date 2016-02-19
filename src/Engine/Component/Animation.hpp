#pragma once

#include "SuperComponent.hpp"

class Entity;

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

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

            /// Starts playing an new animation clip
            /**
             * @param key Key to a animation clip.
             */
            void Start(const std::string key);

            /// Pause the playing animation clip.
            void Pause();

            /// Continues the playing animation clip (if paused).
            void Play();

            /// Stop and reset the playing animation clip.
            void Stop();

            /// Contains data about a key frame.
            struct KeyFrame {
                glm::vec3 position;
                float yaw, pitch, roll;
                float time;
                bool slerp;
                bool slerpTime;
            };

            /// An animation clip constains several key frames.
            struct AnimationClip {
                std::vector<KeyFrame> keyFrameVector;
                unsigned int activeKeyFrame = 0;
                float animationTime = 0.f;
                bool pause = false;
                float speed = 1.f;

                /// Create a key frame in an animation clip.
                /**
                 * @param position Position relative to transform.
                 * @param yaw Relative yaw (in degrees).
                 * @param pitch Relative pitch (in degrees).
                 * @param roll Relative roll (in degrees).
                 * @param time Time for key frame to be fully interpolated (in seconds).
                 * @param slerp Spherical linear interpolation if true, linear interpolation is false.
                 * @param slerpTime Spherical linear time interpolation if true, linear time interpolation is false.
                 */
                void CreateKeyFrame(glm::vec3 position, float yaw, float pitch, float roll, float time, bool slerp, bool slerpTime);
            };

            /// Returns active animation clip.
            /**
             * @return The active animation clip.
             */
            AnimationClip* GetActiveAnimationClip();

            /// Creates an animation clip.
            /**
             * @param key Key to a animation clip.
             */
            AnimationClip* CreateAnimationClip(const std::string key);

            /// Animation matrix, like model matrix but manipulated with animation.
            /**
             * Default: Identity matrix
             */
            glm::mat4 animationMatrix;

            /// Orientation matrix.
            /**
             * Default: Identity matrix
             */
            glm::mat4 orientationMatrix;

        private:
            // Map containing animation clips.
            std::map<std::string, AnimationClip*> mAnimationClipMap;
            
            // Active animation clip.
            /**
             * Default: nullptr
             */
            AnimationClip* mActiveAnimationClip;
        };
}
