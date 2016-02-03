#pragma once

#include <map>
#include <typeinfo>
#include <vector>
#include <iterator>
#include <algorithm>

class Entity;

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
    class RelativeTransform;
    class SuperComponent;
    class Physics;
}

/// Contains a bunch of entities.
class Scene {

    /// %Scene is friend with Entity to gain access to components.
    friend class Entity;

    public:
        /// Create new Scene.
        Scene();

        /// Destructor.
        ~Scene();

        /// Create new Entity in the scene.
        Entity* CreateEntity();
        
        /// Clear the scene of all entities.
        void Clear();

        /// Adds component to map, used externally.
        /**
         *@param component %Component that will be added to map.
         */
        template <typename T> void AddComponentToList(T* component);

        /// Gets number of elements in maps or entity list.
        template <typename T> unsigned int Size() const;
        
        /// Updates all model matrices in %Scene.
        void UpdateModelMatrices();

        /// Gets all components of a specific type.
        /**
         * @return A vector of pointers to all components of the specified scene.
         */
        template <typename T> std::vector<T*> GetAll();

    private:
        // Adds component to list internally.
        void AddComponentToList(Component::SuperComponent* component, const std::type_info* componentType);

        // List of all entities created in this scene.
        std::vector<Entity*> mEntityVector;

        std::map<const std::type_info*, std::vector<Component::SuperComponent*> > mComponents;

};

template<typename T> void Scene::AddComponentToList(T* component){
    const std::type_info* componentType = &typeid(component);
    AddComponentToList(component, componentType);
    return;
}

template <typename T> std::vector<T*> Scene::GetAll() {
    auto found = mComponents.find(&typeid(T*));
    std::vector<T*> returnVector;
    if (found == mComponents.end())
        return returnVector;

    returnVector.reserve(found->second.size());
    for (unsigned int i = 0; i < found->second.size(); ++i)
        returnVector.push_back(static_cast<T*>(found->second[i]));
    return returnVector;
}

// Special case for entity vector
template<> inline unsigned int Scene::Size<Entity>() const {
    return this->mEntityVector.size();
}

// General case
template<typename T> unsigned int Scene::Size() const {
    auto found = mComponents.find(&typeid(T*));
    if (found == mComponents.end())
        return 0;

    return found->second.size();
}

