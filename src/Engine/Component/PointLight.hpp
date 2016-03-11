#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>

class Entity;

namespace Component {
    /// %Component describing a point light.
    class PointLight : public SuperComponent {
        public:
            /// Create new light.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            PointLight(Entity* entity);
            
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
    };
}
