#pragma once

#include "SuperComponent.hpp"

class Entity;

#include <glm/glm.hpp>

namespace Component {
    /// Component providing transformation.
    class Physics : public SuperComponent {
        public:
            /// Create new %Physics.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Physics(Entity* entity);

            /// Destructor.
            ~Physics();

            /// Velocity (in meter/secound).
            /**
             * Default: 0.f, 0.f, 0.f
             */
            glm::vec3 velocity;

            /// Max celocity (in meter/secound).
            /**
            * Default: 20.f
            */
            float maxVelocity;

            /// Angular velocity (in Hz).
            /**
             * Default: 0.f, 0.f, 0.f
             */
            glm::vec3 angularVelocity;

            /// Max angular velocity (in Hz).
            /**
            * Default: 2.f
            */
            float maxAngularVelocity;

            /// Acceleration (in meter/secound^2).
            /**
             * Default: 0.f, 0.f, 0.f
             */
            glm::vec3 acceleration;

            /// Angular acceleration (in rad/secound^2).
            /**
             * Default: 0.f, 0.f, 0.f
             */
            glm::vec3 angularaAcceleration;

            /// Friction factor. Describes the procentage of the velocity that is lost each secound. 
            /**
             * Default: 1.0f
             */
            float velocityDragFactor;

            /// Angular drag factor. Describes the procentage of the rotation that is lost each secound. 
            /**
             * Default: 1.f
             */
            float angularDragFactor;

            /// Gravity factor. 
            /**
             * Default: 0.f
             */
            float gravityFactor;

            /// Moment of inertia. Describes how hard it's to rotate an object.
            /**
             * Default: 1.f, 1.f, 1.f
             * https://en.wikipedia.org/wiki/List_of_moments_of_inertia
             */
            glm::vec3 momentOfInertia;
        };
}
