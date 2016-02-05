#pragma once

class Scene;
class Entity;
class Shader;
class ShaderProgram;

#include "ParticleSystem.hpp"
#include "Particle.vert.hpp"
#include "Particle.geom.hpp"
#include "Particle.frag.hpp"

namespace System {
    /// %System to render particles
    class ParticleRenderSystem
    {
        public:
            /// Create the render system.
            ParticleRenderSystem();

            /// Destructor.
            ~ParticleRenderSystem();

            /// Render
            void Render(Scene& scene, Entity* camera);

        private:
            Shader* mParticleVertShader;
            Shader* mParticleGeomShader;
            Shader* mParticleFragShader;
            ShaderProgram* mParticleShaderProgram;

            // Vertex buffer.
            GLuint mVertexBuffer = 0;
            GLuint mVertexAttribute = 0;
            unsigned int mVertexCount = 0;
    };
}

