#pragma once
#include "SuperComponent.hpp"
#include "../Particles/ParticleSystem.hpp"
#include "glm/glm.hpp"

class Entity;

namespace Component {
    /// Emitter that emits particles.
    class ParticleEmitter : public SuperComponent {
        public:
            /// Type of emitter
            enum EMITTER_TYPE {
                POINT = 0, ///< Point emitter
                CUBOID, ///< Cuboid emitter
            };

            /// Create new particle emitter.
            /**
             * @param entity to which we attatch the emitter
             */
            ParticleEmitter(Entity* entity);

            /// Origin of the particle emitter.
            glm::vec3 origin;

            /// Size of the emitter (area of effect).
            glm::vec3 size;

            /// Minimum time for emitting particle.
            double minEmitTime;

            /// Maximum time for emitting particle.
            double maxEmitTime;

            /// Time to next emit (random value between min and max),
            double timeToNext;

            /// Type of emitter - se enum in ParticleEmitter.hpp.
            int emitterType;

            /// Is the emitter relative to an entity?
            bool relative;

            /// Time the particle has lived (thus far).
            double lifetime;

            /// Pointer to the particleSystem containing the emitter.
            ParticleSystem* particleSystem;

            /// Entity to follow / be relative to.
            Entity* follow;
    };
}

