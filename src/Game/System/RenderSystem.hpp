#pragma once

class Entity;
class ShaderProgram;
class Scene;

class RenderSystem {
    public:

        /// Create new RenderSystem.
        /**
        * @param shaderProgram Shader program that can be run on the GPU.
        */
        RenderSystem(ShaderProgram* shaderProgram = nullptr);

        /// Destructor.
        ~RenderSystem();

        /// Initialize components.
        /**
        * @param shaderProgram Shader program that can be run on the GPU.
        */
        void Init(ShaderProgram* shaderProgram);

        /// Render Scene containing entities.
        /**
        * @param scene Contains a bunch of entities.
        */
        void Render(Scene* scene);

    private:
        ShaderProgram* mShaderProgram;
};
