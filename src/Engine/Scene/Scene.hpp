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

        /// Return number of Entities in Scene.
        unsigned int Size() const;

        /// Clear Scene of all entities
        void Clear();

        /// Return Entity on index, else nullptr.
        Entity* operator[](unsigned int index);

    private:
        std::vector<Entity*> mEntityVec;
};
