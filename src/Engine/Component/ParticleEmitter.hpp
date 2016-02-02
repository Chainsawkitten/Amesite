#pragma once
#include "SuperComponent.hpp"
#include "../Particles/ParticleSystem.hpp"
#include "glm/glm.hpp"

class Entity;

namespace Component {
    /// Emitter that emits particles.
    class ParticleEmitter : public SuperComponent {
        public:
            enum EMITTER_TYPE {
                POINT = 0,
                CUBOID,
            };

            /// Create new particle emitter.
            /**
             * @param entity to which we attatch the emitter
             */
            ParticleEmitter(Entity* entity);

            /// Spawn new particles.
            /**
             * @param time Time since last frame (in seconds).
             * @param particleSystem Particle system to add particles to.
             * @param follow Entity to follow (for relative emitting).
             */
            void Update(double time, Entity* follow, ParticleSystem* particleSystem);

            /// Emit particle.
            virtual void EmitParticle();

            void EmitParticleAt(glm::vec3 position);

            glm::vec3 origin;
            glm::vec3 size;
            double minEmitTime;
            double maxEmitTime;
            double timeToNext;
            int emitterType;

            bool relative;
            double lifetime;
            ParticleSystem* particleSystem;

            Entity* follow;
    };
}

