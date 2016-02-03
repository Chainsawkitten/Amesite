#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../Entity/Entity.hpp"
#include "ParticleType.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture.hpp"

/// Handles particles.
class ParticleSystem {
    public:
        /// A particle in the particle system.
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
        
        /// Create a new particle system with the given properties.
        /**
         * @param particleType The type of particle the system should handle.
         * @param maxParticleCount The maximum amount of particles the particle system can hold.
         */
        ParticleSystem(ParticleType particleType, int maxParticleCount);
        
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
        
        /// Add a particle emitter to the system.
        /**
         * @param emitter Particle emitter to add to the system.
         */
        void AddParticleEmitter(Component::ParticleEmitter* emitter);
        
        /// Remove a particle emitter from the system
        void RemoveParticleEmitter();

        /// Emit a particle a the given position.
        /**
         * @param position Position to emit particle at.
         */
        void EmitParticle(glm::vec3 position);

        /// Decide where the emitter should emit before rendering.
        /**
         * @param emitter for which to calculate emission
         */
        void EmitParticle(Component::ParticleEmitter* emitter);
        
        /// Update all the system's particles, spawn new particles etc.
        /**
         * @param time Time since last frame (in seconds).
         * @param follow Entity to follow (only needs to be sent if there are relative emitters).
         */
        void Update(double time, Entity* follow = nullptr);
        
        /// Render particles.
        /**
         * @param camera Entity through which to render.
         * @param screenSize Size of the screen in pixels.
         */
        void Render(Entity* camera, const glm::vec2& screenSize);
        
    private:
        // Helper functions
        void BindPointData();
        
        Shader* mVertexShader;
        Shader* mGeometryShader;
        Shader* mFragmentShader;
        ShaderProgram* mShaderProgram;
        
        ParticleType mParticleType;
        std::vector<Particle> mParticles;
        std::vector<Component::ParticleEmitter*> mEmitters;
        
        // System properties
        unsigned int mParticleCount;
        unsigned int mMaxParticleCount;
        
        // Vertex buffer.
        GLuint mVertexBuffer = 0;
        GLuint mVertexAttribute = 0;
        unsigned int mVertexCount = 0;
};
