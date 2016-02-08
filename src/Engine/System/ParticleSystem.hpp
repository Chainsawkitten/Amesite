#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Component {
    class ParticleEmitter;
}

class Scene;

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

            /// Get currently active ParticleSystem.
            /**
            * @return The currently active input handler or nullptr.
            */
            static ParticleSystem* GetActiveInstance();

            /// Set as currently active ParticleSystem.
            void SetActive();

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

            /// Emit a particle at the given position.
            /**
             * @param position Position to emit particle at.
             * @param emitter The emitter that emits the particle.
             */
            void EmitParticle(glm::vec3 position, Component::ParticleEmitter* emitter);

            /// Decide where the emitter should emit before rendering.
            /**
             * @param emitter for which to calculate emission
             */
            void EmitParticle(Component::ParticleEmitter* emitter);

            /// Update all the system's particles, spawn new particles etc.
            /**
             * @param scene Scene to update.
             * @param time Time since last frame (in seconds).
             */
             void Update(Scene& scene, double time);
             
        private:
            Scene* mScene;
            static ParticleSystem* mActiveInstance;
            // System properties
            unsigned int mParticleCount;
            unsigned int mMaxParticleCount;
    };

    /// Get currently active ParticleSystem.
    /**
    * @return The currently active ParticleSystem or nullptr.
    */
    ParticleSystem* Particle();
}


