#pragma once

#include "SuperComponent.hpp"

class Entity;

#include <glm\glm.hpp>

namespace Component {
    /// %Component handling a camera lens.
    class Lens : public SuperComponent {
        public:
            /// Create new %Lens.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Lens(Entity* entity);
            
            /// Destructor.
            ~Lens();

            /// Get the projection matrix.
            /**
             * @param screenSize Screen size in pixels.
             * @return The projection matrix.
             */
            glm::mat4 GetProjection(const glm::vec2& screenSize) const;
            
            /// Field of view, in degrees.
            /**
             * Default: 45.0
             */
            float fieldOfView;
            
            /// Near plane.
            /**
             * Default: 0.5
             */
            float zNear;
            
            /// Far plane.
            /**
             * Default: 100.0
             */
            float zFar;
    };
}
