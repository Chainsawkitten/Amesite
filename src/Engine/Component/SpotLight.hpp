#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>

class Entity;

namespace Component {
    /// Component describing a spot light.
    class SpotLight : public SuperComponent {
        public:
            /// Create new light.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            SpotLight(Entity* entity);
            
            /// Color.
            glm::vec3 color;
            
            /// Ambient coefficient.
            float ambientCoefficient;
            
            /// Attenuation.
            float attenuation;
            
            /// Angle of the cone (in degrees).
            float coneAngle;
    };
}
