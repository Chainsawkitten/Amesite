#pragma once

#include <glm\glm.hpp>

namespace Component {

    /// Component handling a camera lens.
    class Parent {
        public:
            /// Create a new lens.
            Parent();
            
            /// Destructor.
            ~Parent();

            /// Get the projection matrix.
            /**
             * @param screenSize Screen size in pixels.
             * @return The projection matrix.
             */
            //glm::mat4 GetProjection(const glm::vec2& screenSize) const;
            
            /// Field of view, in degrees.
            /**
             * Default: 45.0
             */
            //float fieldOfView;
            
            /// Near plane.
            /**
             * Default: 0.5
             */
            //float zNear;
            
            /// Far plane.
            /**
             * Default: 100.0
             */
            //float zFar;
    };

}
