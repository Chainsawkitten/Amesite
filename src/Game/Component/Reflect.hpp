#pragma once

#include <Engine/Component/SuperComponent.hpp>

namespace Component {
    /// %Component to entites that reflect all it collides with.
    class Reflect : public SuperComponent {
        public:
            /// Create %Reflect.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Reflect(Entity* entity);
            
            /// Destructor.
            ~Reflect();

            /// Faction, ID of the faction this health belongs to (players, enemies).
            /**
             * Default: 1
             */
            int faction;
    };
}
