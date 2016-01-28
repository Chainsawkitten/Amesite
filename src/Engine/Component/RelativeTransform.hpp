#pragma once

#include "Transform.hpp"

class Entity;

namespace Component {

    /// Component handling a parent entity.
    class RelativeTransform : public Transform {
        public:
            /// Create a new lens.
            RelativeTransform();
            
            /// Destructor.
            ~RelativeTransform();

            /// Get model matrix.
            /**
            * @return The entity's model matrix, containing translation, scaling and rotation.
            */
            glm::mat4 GetOrientation() const;
            
            /// Parent Entity.
            /**
             * Default: nullptr
             */
            Entity* parentEntity;
    };

}
