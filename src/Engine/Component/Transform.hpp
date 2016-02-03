#pragma once

#include "SuperComponent.hpp"

class Entity;

#include <glm/glm.hpp>

namespace Component {
    /// Component providing transformation.
    class Transform : public SuperComponent {
        public:
            /// Create new %Transform.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Transform(Entity* entity);

            /// Destructor.
            virtual ~Transform();

            /// Update model matrix.
            virtual void UpdateModelMatrix();

            /// Get orientation matrix.
            /**
             * @return The entity's orientation matrix.
             */
            glm::mat4 GetOrientation() const;

            /// Set rotation by an offset.
            /**
             * Rotate the object relative to its current rotation.
             * @param yaw Relative yaw (in degrees).
             * @param pitch Relative pitch (in degrees).
             * @param roll Relative roll (in degrees).
             */
            void Rotate(float yaw, float pitch, float roll);

            /// Move the current position by an offset.
            /**
             * Move the object relative to its current position.
             * @param offset Offset by which to move by.
             */
            void Move(const glm::vec3 &offset);

            /// Move the current position by an offset in x,y,z.
            /**
             * Move the object relative to its current position.
             * @param x Offset along x-axis.
             * @param y Offset along y-axis.
             * @param z Offset along z-axis.
             */
            void Move(float x, float y, float z);
        
            /// Position in the world.
            /**
             * Default: 0.f, 0.f, 0.f
             */
            glm::vec3 position;
        
            /// Scale.
            /**
             * Default: 1.f, 1.f, 1.f
             */
            glm::vec3 scale;
        
            /// Yaw (in degrees).
            /**
             * Default: 0.f
             */
            float yaw;
        
            /// Pitch (in degrees).
            /**
             * Default: 0.f
             */
            float pitch;
        
            /// Roll (in degrees).
            /**
             * Default: 0.f
             */
            float roll;

            /// Model Matrix
            /**
             * Default: Identity matrix
             */
            glm::mat4 modelMatrix;
        };
}
