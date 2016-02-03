#pragma once

#include "Transform.hpp"

class Entity;

namespace Component {
    /// Component providing transformation relative to a parent Entity.
    class RelativeTransform : public Transform {
        public:
            /// Create new %RelativeTransform.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            RelativeTransform(Entity* entity);
            
            /// Destructor.
            ~RelativeTransform();

            /// Update model matrix.
            void UpdateModelMatrix();
            
            /// Parent Entity.
            /**
             * Default: nullptr
             */
            Entity* parentEntity;
    };
}
