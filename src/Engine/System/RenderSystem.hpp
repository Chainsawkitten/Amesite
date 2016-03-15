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
             * @param clippingPlane Clipping plane.
             */
            void Render(Scene& scene, RenderTarget* renderTarget, const glm::vec4& clippingPlane = glm::vec4(0.f, 0.f, 0.f, 0.f));

        private:
            Shader* mVertexShader;
            Shader* mFragmentShader;
            ShaderProgram* mShaderProgram;

            DeferredLighting* mDeferredLighting;


            // Particle Render system.
            ParticleRenderSystem particleRenderSystem;
    };
}
