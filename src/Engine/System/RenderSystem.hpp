#pragma once

class Entity;
class Scene;
class Shader;
class ShaderProgram;
class DeferredLighting;

namespace System {
    /// System to render a scene.
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

            Shader* mParticleVertShader;
            Shader* mParticleGeomShader;
            Shader* mParticleFragShader;
            ShaderProgram* mParticleShaderProgram;
        };     
            DeferredLighting* mDeferredLighting;
    };
}
