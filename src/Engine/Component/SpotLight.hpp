#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>

class Entity;

namespace Component {
    /// %Component describing a spot light.
    class SpotLight : public SuperComponent {
        public:
            /// Create new light.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            SpotLight(Entity* entity);
            
            /// Color.
            /**
             * Default: 1.f, 1.f, 1.f
             */
            glm::vec3 color;

            /// Ambient coefficient.
            /**
             * Default: 0.f
             */
            float ambientCoefficient;

            /// Attenuation.
            /**
             * Default: 1.f
             */
            float attenuation;

            /// Intensity.
            /**
             * Default: 1.f
             */
            float intensity;
            
            /// Angle of the cone (in degrees).
            /**
             * Default: 15.f
             */
            float coneAngle;
    };
}
