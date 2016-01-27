#pragma once

class Entity;

class RenderSystem {
    public:

        /// Create new RenderSystem.
        RenderSystem();

        /// Destructor.
        ~RenderSystem();

        /// Render array of Entities .
        /**
        * @param entityArr Array of Entities.
        * @param nrOfEntities Number of entities in entityArr.
        */
        void Render(Entity* entityArr, unsigned int nrOfEntities);

    private:
};