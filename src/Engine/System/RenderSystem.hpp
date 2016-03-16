#pragma once

#include "ParticleRenderSystem.hpp"

class Entity;
class Scene;
class Shader;
class ShaderProgram;
class DeferredLighting;
class RenderTarget;

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
             * @param renderTarget Target to render to.
             */
            void Render(Scene& scene, RenderTarget* renderTarget);

        private:
            Shader* mVertexShader;
            Shader* mFragmentShader;
            ShaderProgram* mShaderProgram;

            DeferredLighting* mDeferredLighting;
    };
}
