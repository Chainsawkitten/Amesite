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
            /// Create new render system.
            RenderSystem();

            /// Destructor.
            ~RenderSystem();

            /// Render Scene containing entities.
            /**
             * @param scene Contains a bunch of entities.
             */
            void Render(const Scene& scene);

        private:
            Shader* mVertShader;
            Shader* mFragShader;
            ShaderProgram* mShaderProgram;
            
            DeferredLighting* mDeferredLighting;
    };
}
