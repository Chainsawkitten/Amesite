#pragma once

class Entity;
#include <vector>

/// Contains a bunch of entities.
class Scene {
    public:
        /// Create new Scene.
        Scene();

        /// Destructor.
        ~Scene();

        /// Create new Entity in Scene.
        Entity* CreateEntity();

        /// Get number of entities in Scene.
        /**
        * @return Number of entities in Scene
        */
        unsigned int Size() const;

        /// Clear Scene of all entities.
        void Clear();

        /// Get Entity on index, else nullptr.
        /**
        * @param index The index of the Entity.
        * @return Entity on index, else nullptr.
        */
        Entity* operator[](unsigned int index) const;

    private:
        std::vector<Entity*> mEntityVec;
};
