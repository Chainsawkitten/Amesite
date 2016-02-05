#pragma once

#include <Engine/Component/SuperComponent.hpp>

namespace Component {
    ///Component providing player control.
    class Damage : public SuperComponent {
        public:
            /// Create new %Health.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Damage(Entity* entity);

            /// Destructor.
            ~Damage();

            /// Amount of damage
            /**
             * Default: 10.f
             */
            float damageAmount;
    };
}
