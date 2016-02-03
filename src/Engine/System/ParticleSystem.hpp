#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../Entity/Entity.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture.hpp"

namespace System {
    /// Handles particles.
    class ParticleSystem {
        public:
            /// A particle in the particle emitter.
            struct Particle {
                /// Position.
                glm::vec3 worldPos;

                /// Size.
                glm::vec2 size;

                /// Life (in seconds)
                float life;

                /// Lifetime (in seconds)
                float lifetime;

                /// Initial velocity.
                glm::vec3 velocity;
            };

            /// Create a new particle system.
            ParticleSystem();

            /// Destructor.
            ~ParticleSystem();

            /// Get the amount of particles.
            /**
             * @return How many particles currently exist.
             */
            unsigned int ParticleCount() const;

            /// Get the maximum amount of particles.
            /**
             * @return Maximum amount of particles.
             */
            unsigned int MaxParticleCount() const;

            /// Emit a particle a the given position.
            /**
             * @param position Position to emit particle at.
             */
            void EmitParticle(Scene& scene, glm::vec3 position, Component::ParticleEmitter* emitter);

            /// Decide where the emitter should emit before rendering.
            /**
             * @param emitter for which to calculate emission
             */
            void EmitParticle(Scene& scene, Component::ParticleEmitter* emitter);

            /// Update all the system's particles, spawn new particles etc.
            /**
             * @param time Time since last frame (in seconds).
             * @param follow Entity to follow (only needs to be sent if there are relative emitters).
             */
             void Update(Scene& scene, double time);
        private:
            // System properties
            unsigned int mParticleCount;
            unsigned int mMaxParticleCount;
    };
}
