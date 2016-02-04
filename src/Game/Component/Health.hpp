#pragma once

#include <Engine/Component/SuperComponent.hpp>

namespace Component {
    ///Component providing player control.
    class Health : public SuperComponent {
        public:
            /// Create new %Health.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Health(Entity* entity);

            /// Destructor.
            ~Health();

            /// Health
            /**
             * Default: 100.f
             */
            float health;

            /// Max health
            /**
            * Default: 100.f
            */
            float maxHealth;

            /// Toughness, procent of damage taken.
            /**
             * Default: 1.f
             */
            float toughness;
            
            /// Regen cooldown, time until entity will regain health (in seconds).
            /**
             * Default: 1.f
             */
            float maxCooldown;

            /// Regen amount, amount of health regenerated each secound (hp/sec).
            /**
             * Default: 20.f
             */
            float regenAmount;

            // Cooldown, time left until regaining helth again. If cooldown < 0.f entity regan health;
            /**
             * Default: 0.f
             */
            float cooldown;
    };
}
