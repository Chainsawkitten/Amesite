#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>

class Entity;

namespace Component {
    /// %Component describing a fake point light.
    class FakePointLight : public SuperComponent {
        public:
            /// Create new fake light.
            /**
             * @param entity Pointer to which entity this component corresponds.
             */
            FakePointLight(Entity* entity);
            
            /// Color.
            /**
             * Default: 1.f, 1.f, 1.f
             */
            glm::vec3 color;
            
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
