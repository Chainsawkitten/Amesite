#pragma once

#include <glm/glm.hpp>
#include <vector>

class Entity;
class Scene;
namespace Component {
    class Animation;
}

namespace System {
    /// %System to animate.
    class AnimationSystem {
        public:
            /// Create animation system.
            AnimationSystem();

            /// Destructor.
            ~AnimationSystem();

            /// Update Scene containing entities.
            /**
             * @param scene Contains a bunch of entities.
             * @param deltaTime Time since last frame (in seconds).
             */
            void Update(Scene& scene, float deltaTime);

        private:
            // Get orientation matrix.
            glm::mat4 GetOrientation(float yaw, float pitch, float roll);

            // Linear interpolation for floats.
            float LerpTime(float animationTime, float frameTime);

            // Spherical linear interpolation for floats.
            float SlerpTime(float animationTime, float frameTime);

            // Linear interpolation for vectors.
            glm::vec3 Lerp(glm::vec3 p0, glm::vec3 p1, float t);

            // Spherical linear interpolation for vectors.
            glm::vec3 Slerp(glm::vec3 p0, glm::vec3 p1, float t);
            
            float mDeltaTime;
            
            // Update part of the component vector.
            void UpdatePart(std::vector<Component::Animation*>& animations, std::size_t begin, std::size_t length);
    };
}
