#pragma once

class Entity;
class ShaderProgram;
class Scene;

class RenderSystem {
    public:

        /// Create new RenderSystem.
        RenderSystem(ShaderProgram* shaderProgram = nullptr);

        /// Destructor.
        ~RenderSystem();

        /// Initialize components.
        void Init(ShaderProgram* shaderProgram);

        /// Render Scene containing entities.
        /**
        * @param scene Contains a bunch of entities.
        */
        void Render(Scene* scene);

    private:
        ShaderProgram* mShaderProgram;
};
