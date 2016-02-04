#pragma once
#include "SuperComponent.hpp"
#include "glm/glm.hpp"

class Entity;
class Texture;

namespace Component {
    /// Emitter that emits particles.
    class ParticleEmitter : public SuperComponent {
        public:
            /// Type of emitter
            enum EMITTER_TYPE {
                POINT = 0, ///< Point emitter
                CUBOID, ///< Cuboid emitter
            };

            /// Defines a kind of particle.
            struct ParticleType {
                /// Texture to apply to the particle.
                Texture* texture = nullptr;

                /// The minimum initial velocity in all directions.
                glm::vec3 minVelocity;

                /// The maximum initial velocity in all directions.
                glm::vec3 maxVelocity;

                /// The minimum lifetime of the particle (in seconds).
                float minLifetime = 0.f;

                /// The maximum lifetime of the particle (in seconds).
                float maxLifetime = 0.f;

                /// The minimum size of the particle.
                glm::vec2 minSize = glm::vec2(1.f, 1.f);

                /// The maximum size of the particle.
                glm::vec2 maxSize = glm::vec2(1.f, 1.f);

                /// Whether to scale all axes individually or uniformly.
                bool uniformScaling;

                /// Alpha at the beginning of the particle's life.
                float startAlpha = 0.f;

                /// Alpha in the middle of the particle's life.
                float midAlpha = 1.f;

                /// Alpha at the end of the particle's life.
                float endAlpha = 0.f;

                /// Blend color.
                glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
            };

            /// Create new particle emitter.
            /**
             * @param entity to which we attatch the emitter
             */
            ParticleEmitter(Entity* entity);

            /// Particletype
            ParticleType particleType;

            /// Origin of the particle emitter.
            /**
            * Default: 0.f, 0.f, 0.f
            */
            glm::vec3 origin;

            /// Size of the emitter (area of effect).
            /**
            * Default: 0.f, 0.f, 0.f
            */
            glm::vec3 size;

            /// Minimum time for emitting particle.
            /**
            * Default: 0.03
            */
            double minEmitTime;

            /// Maximum time for emitting particle.
            /**
            * Default: 0.05
            */
            double maxEmitTime;

            /// Time to next emit (random value between min and max),
            /**
            * Default: 5.0
            */
            double timeToNext;

            /// Type of emitter - se enum in ParticleEmitter.hpp.
            /**
            * Default: 0
            */
            int emitterType;

            /// Is the emitter relative to an entity?
            /**
            * Default: false
            */
            bool relative;

            /// Time the particle has lived (thus far).
            /**
            * Default: 0.0
            */
            double lifetime;

            /// Entity to follow / be relative to.
            /**
            * Default: nullptr
            */
            Entity* follow;
    };
}

