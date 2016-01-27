#pragma once

#include <glm\glm.hpp>

namespace Component {

    /// Component handling a camera lens.
    class Lens {
        public:
            /// Create a new lens.
            Lens();
            
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
            float mFieldOfView;
            
            /// Near plane.
            /**
             * Default: 0.5
             */
            float mZNear;
            
            /// Far plane.
            /**
             * Default: 100.0
             */
            float mZFar;
    };

}
