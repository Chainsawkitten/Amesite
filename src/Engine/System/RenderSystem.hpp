#pragma once

#include "ParticleRenderSystem.hpp"

class Entity;
class Scene;
class Shader;
class ShaderProgram;
class DeferredLighting;

namespace System {
    /// %System to render a scene.
    class RenderSystem {
        public:
            /// Create render system.
            RenderSystem();

            /// Destructor.
            ~RenderSystem();

            /// Render Scene containing entities.
            /**
             * @param scene Contains a bunch of entities.
             */
            void Render(Scene& scene);

        private:
            Shader* mVertexShader;
            Shader* mFragmentShader;
            ShaderProgram* mShaderProgram;

            DeferredLighting* mDeferredLighting;


            // Particle Render system.
            ParticleRenderSystem particleRenderSystem;
    };
}
