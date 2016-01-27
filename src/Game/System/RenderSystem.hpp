#pragma once

class Entity;
class ShaderProgram;

class RenderSystem {
    public:

        /// Create new RenderSystem.
        RenderSystem(ShaderProgram* shaderProgram = nullptr);

        /// Destructor.
        ~RenderSystem();

        /// Initialize components.
        void Init(ShaderProgram* shaderProgram);

        /// Render array of Entities .
        /**
        * @param entityArr Array of Entities.
        * @param nrOfEntities Number of entities in entityArr.
        */
        void Render(Entity** entityArr, unsigned int nrOfEntities);

    private:
        ShaderProgram* mShaderProgram;
};