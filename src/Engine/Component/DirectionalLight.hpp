#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>

class Entity;

namespace Component {
    /// %Component describing a directional light (like a sun).
    class DirectionalLight : public SuperComponent {
        public:
            /// Create new light.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            DirectionalLight(Entity* entity);
            
            /// Color.
            glm::vec3 color;
            
            /// Ambient coefficient.
            float ambientCoefficient;
    };
}
