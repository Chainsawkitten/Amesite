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
    class Collider2DCircle;
    //class Collider2DRectangle;
}

/// Contains a bunch of entities.
class Scene {

    /// %Scene is friend with Enity to gain access to components
    friend class Entity;

    public:
        /// Create new Scene.
        Scene();

        /// Destructor.
        ~Scene();

        /// Create new Entity in Scene.
        Entity* CreateEntity();
        /// Clear Scene of all items.
        void Clear();

        ///Adds component to map, used externally.
        /**
         *@param component component that will be added to map.
         */
        template <typename T> void AddComponentToList(T* component);

        ///Gets number of elements in maps or entity list.
        template <typename T> unsigned int Size() const;
        
        /// Updates all model matrices in %Scene.
        void UpdateModelMatrices();

        ///Gets all components of a specific type.
        /**
         * @param returnVector Vector that will keep components.
         */
        //template <typename T> void GetAll(std::vector<T*> &returnVector);

        ///Gets all components of a specific type, returns a vector.
        template <typename T> std::vector<T*> GetAll();

    private:
        ///Adds component to list internally.
        /**
         * @param component The component that will be added.
         * @param componentType The type of the component.
         */
        void AddComponentToList(Component::SuperComponent* component, const type_info* componentType);

        ///List of all entities created in this scene.
        std::vector<Entity*> mEntityVector;

        ///multimap that maps component type to multiple components.
        std::map<const std::type_info*, std::vector<Component::SuperComponent*> > mComponents;

};

template<typename T> void Scene::AddComponentToList(T* component){
    const type_info* componentType = &typeid(component);
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

template<> inline std::vector<Entity*> Scene::GetAll() {
    return mEntityVector;
}
