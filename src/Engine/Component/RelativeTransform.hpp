#pragma once

#include "Transform.hpp"

class Entity;

namespace Component {
    /// %Component providing transformation relative to a parent Entity.
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

            /// Get scale of the entity.
            /**
             * @return The entity's scale relative to a parent.
             */
            glm::vec3 GetWorldScale() const;
            
            /// Get roation of the entity.
            /**
             * @return The entity's rotation (yaw, pitch and roll in degrees).
             */
            glm::vec3 GetWorldRotation() const;

            /// Parent Entity.
            /**
             * Default: nullptr
             */
            Entity* parentEntity;
    };
}
