#pragma once

#include "../Component/ParticleEmitter.hpp"
#include <glm/glm.hpp>

/// Emitter that emits particles in a cuboid area.
class CuboidParticleEmitter : public Component::ParticleEmitter {
    public:
        /// Create new CuboidParticleEmitter.
        /**
         * @param origin Origin.
         * @param size Size.
         * @param minEmitTime The minimum time between each emitted particle.
         * @param maxEmitTime The maximum time between each emitted particle.
         * @param relative Whether to emit particles relative to the camera position.
         */
        CuboidParticleEmitter(glm::vec3 origin, glm::vec3 size, Entity* entity);
        
        /// Emit particle.
        void EmitParticle();
        
    private:
        glm::vec3 mOrigin;
        glm::vec3 mSize;
};

