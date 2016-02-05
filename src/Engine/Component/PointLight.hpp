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
            glm::vec3 color;
            
            /// Ambient coefficient.
            float ambientCoefficient;
            
            /// Attenuation.
            float attenuation;
    };
}
