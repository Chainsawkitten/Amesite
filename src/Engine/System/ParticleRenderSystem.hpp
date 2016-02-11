#pragma once

class Scene;
class Entity;
class Shader;
class ShaderProgram;
class Texture;
class Texture2D;

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
            void Render(Scene& scene, Entity* camera, const glm::vec2& screenSize);

        private:
            Shader* mParticleVertShader;
            Shader* mParticleGeomShader;
            Shader* mParticleFragShader;
            ShaderProgram* mParticleShaderProgram;

            // When rows are added to texture atlas this needs to be updated.
            float mTextureAtlasNumRows;

            // Texture atlas where the particle textures are found.
            Texture2D* mTextureAtlas;

            // Vertex buffer.
            GLuint mVertexBuffer = 0;
            GLuint mVertexAttribute = 0;
            unsigned int mVertexCount = 0;
    };
}

