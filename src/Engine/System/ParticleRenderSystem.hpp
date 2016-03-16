#pragma once

class Scene;
class Entity;
class Shader;
class ShaderProgram;
class Texture;
class Texture2D;

#include "ParticleSystem.hpp"

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
            /**
             * @param scene %Scene containing particles to render.
             * @param camera Camera through which to render.
             * @param screenSize Size of the screen in pixels.
             * @param clippingPlane Clipping plane.
             */
            void Render(Scene& scene, Entity* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane = glm::vec4(0.f, 0.f, 0.f, 0.f));

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

