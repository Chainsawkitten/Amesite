#pragma once

#include <Engine/Component/SuperComponent.hpp>

namespace Component {
    /// %Component dealing damage to health components.
    class Damage : public SuperComponent {
        public:
            /// Create new damage component.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Damage(Entity* entity);

            /// Destructor.
            ~Damage();

            /// Amount of damage dealt.
            /**
             * Default: 10.f
             */
            float damageAmount;

            /// Remove Entity to which this %Component corresponds if true.
            /**
             * Default: true
             */
            bool removeOnImpact;

            /// Faction, ID of the faction this health belongs to (players, enemies).
            /**
             * Default: 0
             */
            int faction;
    };
}
