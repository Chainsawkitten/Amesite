#pragma once

#include <vector>

class Scene;
namespace Component {
    class GridCollide;
}

namespace GameObject {
    class Cave;
}

namespace System {
    /// %System to check if entity collide with map
    class GridCollideSystem {
        public:
            /// Create grid collide system.
            GridCollideSystem();
            
            /// Destructor.
            ~GridCollideSystem();
            
            /// Update Scene containing entities. Updates life time components.
            /**
             * @param scene The scene to update.
             * @param deltaTime Time since last frame (in seconds).
             * @param cave Cave to check collision.
             */
            void Update(Scene& scene, float deltaTime, GameObject::Cave& cave);
            
        private:
            float mDeltaTime;
            GameObject::Cave* mCave;
            
            // Update part of the component vector.
            void UpdatePart(std::vector<Component::GridCollide*>& gridCollides, std::size_t begin, std::size_t length);
    };
}
