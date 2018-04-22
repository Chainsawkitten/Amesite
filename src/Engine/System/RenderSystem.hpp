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
             * @param screenSize Size of the render target in pixels.
             * @param clippingPlane Clipping plane.
             * @param showLightVolumes Show light volumes as spheres.
             */
            void Render(Scene& scene, RenderTarget* renderTarget, const glm::vec2& screenSize, const glm::vec4& clippingPlane = glm::vec4(0.f, 0.f, 0.f, 0.f), bool showLightVolumes = false);
            
            /// Visualize the light tiles.
            void ShowLightTiles();
            
        private:
            Shader* mVertexShader;
            Shader* mFragmentShader;
            ShaderProgram* mShaderProgram;

            DeferredLighting* mDeferredLighting;
    };
}
