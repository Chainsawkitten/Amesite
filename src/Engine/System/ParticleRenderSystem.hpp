#pragma once

class Scene;

#include <>

namespace System {
    /// System to render particles
    class ParticleRenderSystem
    {
    public:
        /// Create the render system.
        ParticleRenderSystem();

        /// Destructor.
        ~ParticleRenderSystem();

        /// Render
        void Render(const Scene& scene);
    private:
        // Vertex buffer.
        GLuint mVertexBuffer = 0;
        GLuint mVertexAttribute = 0;
        unsigned int mVertexCount = 0;
    };
}

